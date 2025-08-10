/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa <mustafa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:59 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/10 22:21:13 by mustafa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static char	*generate_tmp_filename(int index)
{
	char	*index_str;
	char	*filename;

	index_str = ft_itoa(index);
	filename = ft_strjoin("/tmp/minishell_heredoc_", index_str);
	free(index_str);
	return (filename);
}

static void	run_heredoc_child(t_redirection *redir, int fd)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->infile) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	exit(0);
}

static char	*wait_for_heredoc(pid_t pid, char *tmp_filename)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_last_exit = 130;
		write(STDOUT_FILENO, "\n", 1);
		unlink(tmp_filename);
		free(tmp_filename);
		return (NULL);
	}
	return (tmp_filename);
}

static char	*handle_heredoc(t_redirection *redir, int index)
{
	int		fd;
	char	*tmp_filename;
	pid_t	pid;
	void	(*original_sigint_handler)(int);
	char	*result;

	tmp_filename = generate_tmp_filename(index);
	fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		free(tmp_filename);
		return (NULL);
	}
	original_sigint_handler = signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		run_heredoc_child(redir, fd);
	close(fd);
	result = wait_for_heredoc(pid, tmp_filename);
	signal(SIGINT, original_sigint_handler);
	return (result);
}

char	**process_heredocs_in_cmd(t_cmd *curr_cmd, char **tmp_files, int *i)
{
	t_redirection	*curr_redir;
	char			*old_infile;

	curr_redir = curr_cmd->redirections;
	while (curr_redir)
	{
		if (curr_redir->type == T_HEREDOC)
		{
			tmp_files[*i] = handle_heredoc(curr_redir, *i);
			if (!tmp_files[*i])
				return (NULL);
			old_infile = curr_redir->infile;
			if (old_infile)
				free(old_infile);
			curr_redir->infile = tmp_files[*i];
			curr_redir->type = T_REDIR_IN;
			(*i)++;
		}
		curr_redir = curr_redir->next;
	}
	return (tmp_files);
}
