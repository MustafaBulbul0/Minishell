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

char	*handle_heredoc(t_redirection *redir, int index)
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
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, redir->infile) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (tmp_filename);
}
