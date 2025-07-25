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

static char	*handle_heredoc(t_redirection *redir, int index)
{
	int		fd;
	char	*line;
	char	*tmp_filename;

	tmp_filename = generate_tmp_filename(index);
	fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		free(tmp_filename);
		return (NULL);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->infile) == 0)
			free(line);
		if (!line || ft_strcmp(line, redir->infile) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (tmp_filename);
}

char	**process_heredocs_in_cmd(t_cmd *curr_cmd, char **tmp_files, int *i)
{
	t_redirection	*curr_redir;

	curr_redir = curr_cmd->redirections;
	while (curr_redir)
	{
		if (curr_redir->type == T_HEREDOC)
		{
			tmp_files[*i] = handle_heredoc(curr_redir, *i);
			if (!tmp_files[*i])
			{
				ft_free_split(tmp_files);
				return (NULL);
			}
			free(curr_redir->infile);
			curr_redir->infile = ft_strdup(tmp_files[*i]);
			curr_redir->type = T_REDIR_IN;
			(*i)++;
		}
		curr_redir = curr_redir->next;
	}
	return (tmp_files);
}
