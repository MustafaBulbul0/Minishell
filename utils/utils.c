#include "./../minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*copy;

	copy = malloc(n + 1);
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		copy[i] = s[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

int	is_numeric(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	list_len(t_cmd	*list)
{
	int	i;

	i = 0;
	while (list)
	{
		i++;
		list = list->next;
	}
	return (i);
}

void	ft_free_split(char	**arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		free(arg[i]);
		i++;
	}
	free(arg);
}

char	**envlist_to_array(t_envlist *env)
{
	int			count;
	int			i;
	t_envlist	*tmp;
	char		**arr;

	i = 0;
	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		arr[i] = ft_strjoin_three(tmp->key, "=", tmp->value);
		if (!arr[i])
			return (NULL);
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	return (result);
}

void	handle_redirections_fd(t_cmd *cmd)
{
	t_redirection	*redir;
	int				fd;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->infile)
		{
			fd = open(redir->infile, O_RDONLY);
			if (fd < 0)
			{
				perror(redir->infile);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (redir->outfile)
		{
			if (redir->append)
				fd = open(redir->outfile, O_WRONLY | O_CREAT | O_APPEND);
			else
				fd = open(redir->outfile, O_WRONLY | O_CREAT | O_TRUNC);
			if (fd < 0)
			{
				perror(redir->outfile);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
}
