/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa <mustafa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:59 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/14 23:31:13 by mustafa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	wait_for_heredoc_child(pid_t pid);
static void	handle_heredoc_child(t_redirection *redir, int write_fd,
				t_envlist *env, t_cmd *all_commands, t_token *all_tokens);
static int	setup_heredoc(t_redirection *redir, t_envlist *env,
				t_cmd *all_commands, t_token *all_tokens);

static char	*expand_heredoc_line(char *line, t_envlist *env)
{
	char	*expanded_line;

	if (ft_strchr(line, '$'))
	{
		expanded_line = expand_variable(line, env);
		free(line);
		return (expanded_line);
	}
	return (line);
}

static void	handle_heredoc_child(t_redirection *redir, int write_fd,
	t_envlist *env, t_cmd *all_commands, t_token *all_tokens)
{
	char	*line;
	int		is_quoted;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	is_quoted = (ft_strchr(redir->infile, '\'')
			|| ft_strchr(redir->infile, '"'));
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->infile) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		if (!is_quoted)
			line = expand_heredoc_line(line, env);
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	free_commands(all_commands);
	free_tokens(all_tokens);
	free_env(env);
	exit(0);
}

static int	setup_heredoc(t_redirection *redir, t_envlist *env,
	t_cmd *all_commands, t_token *all_tokens)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (perror("minishell: pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		handle_heredoc_child(redir, pipe_fd[1], env, all_commands, all_tokens);
	}
	close(pipe_fd[1]);
	redir->heredoc_fd = pipe_fd[0];
	status = wait_for_heredoc_child(pid);
	return (status);
}

static int	wait_for_heredoc_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_last_exit = 130;
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	return (0);
}

int	preprocess_heredocs(t_cmd *cmd_list, t_envlist *env, t_token *all_tokens)
{
	t_cmd			*curr_cmd;
	t_redirection	*curr_redir;

	curr_cmd = cmd_list;
	while (curr_cmd)
	{
		curr_redir = curr_cmd->redirections;
		while (curr_redir)
		{
			if (curr_redir->type == T_HEREDOC)
			{
				if (setup_heredoc(curr_redir, env, cmd_list, all_tokens) != 0)
					return (1);
			}
			curr_redir = curr_redir->next;
		}
		curr_cmd = curr_cmd->next;
	}
	return (0);
}
