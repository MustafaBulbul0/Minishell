#include "./../minishell.h"

int	special_counter(int start, char *input);
char	*edit_sep(char *sep);
int	is_qote(char *input, int start, int i);
t_cmd	*creat_struct(char **sep);
t_cmd	*token_separate(char *input)
{
	char	**sep;
	int		start;
	int		len;
	int		i;

	input = ft_strtrim(input, " ");
	sep = (char **)malloc(sizeof(char *) * (ft_strlen(input) + 1));
	start = 0;
	i = 0;
	while (input[start])
	{
		len = special_counter(start, input);
		sep[i] = ft_substr(input, start, len);
		sep[i] = edit_sep(sep[i]);
		start += len;
		while (input[start] == ' ')
			start++;
		i++;
	}
	sep[i] = NULL;
	return (creat_struct(sep));
}

int	special_counter(int start, char *input)
{
	int	i;

	i = 0;
	while (input[start + i] == ' ')
		i++;
	i = is_qote(input, start, i);
	if (input[start + i] == '>')
	{
		if (input[start + i + 1] == '>')
			return (2);
		return (1);
	}
	else if (input[start + i] == '<')
	{
		if (input[start + i + 1] == '<')
			return (2);
		return (1);
	}
	else if (input[start + i] == '|')
		return (1);
	else
	{
		while (input[start + i] && input[start + i] != ' '
			&& input[start + i] != '<' && input[start + i] != '>'
			&& input[start + i] != '|')
		{
			i = is_qote(input, start, i);
			i++;
		}
		return (i);
	}
	return (i);
}

int	is_qote(char *input, int start, int i)
{
	if (input[start + i] == '"')
	{
		i++;
		while (input[start + i] && input[start + i] != '"')
			i++;
		if (input[start + i] == '"')
			i++;
	}
	else if (input[start + i] == '\'')
	{
		i++;
		while (input[start + i] && input[start + i] != '\'')
			i++;
		if (input[start + i] == '\'')
			i++;
	}
	return (i);
}

char	*edit_sep(char *sep)
{
	int		s_qote;
	int		d_qote;
	int		i;
	int		j;
	char	*new_sep;

	s_qote = 0;
	d_qote = 0;
	i = 0;
	j = 0;
	new_sep = (char *)malloc(sizeof(sep));
	while (sep[i])
	{
		while (sep[i] == '"' && s_qote % 2 == 0)
		{
			d_qote++;
			i++;
			if (sep[i] == '"' && d_qote % 2 == 1)
			{
				i++;
				d_qote++;
			}
		}
		while (sep[i] == '\'' && d_qote % 2 == 0)
		{
			s_qote++;
			i++;
			if (sep[i] == '\'' && s_qote % 2 == 1)
			{
				i++;
				s_qote++;
			}
		}
		new_sep[j] = sep[i];
		j++;
		i++;
	}
	new_sep[j] = '\0';
	return (new_sep);
}

static int	is_redirections(char *input)
{
	int	len;

	len = ft_strlen(input);
	if (ft_strncmp(input, ">", 2) == 0 || ft_strncmp(input, ">>", 3) == 0)
			return (1);
	else if (ft_strncmp(input, "<", 2) == 0 || ft_strncmp(input, "<<", 3) == 0)
			return (2);
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "echo", 5)
		|| !ft_strncmp(cmd, "pwd", 4)
		|| !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6)
		|| !ft_strncmp(cmd, "exit", 5));
}

int	is_cmd(char **sep, t_cmd *node)
{
	int	i;

	i = 0;
	if (node->cmd != NULL)
		return (-1);
	while (sep[i])
	{
		if (access(sep[i], F_OK) == 0 || is_builtin(sep[i]))
		{
			node->cmd = sep[i];
			return (i);
		}
		i++;
	}
	return (-1);
}

void	add_redirection(t_redirection *redirections, char **sep, int i)
{
	t_redirection	*node;
	t_redirection	*curr;

	curr = redirections;
	node = (t_redirection *)ft_calloc(1, sizeof(t_redirection));
	while (curr)
		curr = curr->next;
	curr = node;
	if (is_redirections(sep[i]) == 1)
	{
		if (ft_strlen(sep[i]) == 2)
			node->append = 1;
		node->outfile = sep[i + 1];
	}
	else if (is_redirections(sep[i]) == 2)
	{
		if (ft_strlen(sep[i]) == 2)
			node->append = 1;
		node->outfile = sep[i -1];
	}
}

t_cmd	*creat_struct(char **sep)
{
	t_cmd	*head;
	t_cmd	*curr;
	int		i;

	i = 0;
	head = NULL;
	curr = NULL;
	while (sep[i])
	{
		if (!curr)
		{
			curr = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
			head = curr;
		}
		else if (ft_strncmp(sep[i], "|", 2) == 0)
		{
			curr->next = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
			curr = curr->next;
			i++;
			continue ;
		}
		if (is_redirections(sep[i]) == 1)
		{
			curr->redirection = 1;
			if (sep[i + 1])
			{
				curr->outfile = sep[i + 1];
				if (ft_strncmp(sep[i], ">>", 2) == 0)
					curr->append = 1;
				i += 2;
				continue ;
			}
			else
			{
				printf("bash: syntax error near unexpected token `newline'\n");
				return (NULL);
			}
		}
		else if (is_redirections(sep[i]) == 2)
		{
			curr->redirection = 1;
			if (sep[i + 1])
			{
				curr->infile = sep[i + 1];
				if (ft_strncmp(sep[i], "<<", 2) == 0)
					curr->append = 2;
				i += 2;
				continue;
			}
			else
			{
				printf("bash: syntax error near unexpected token `newline'\n");
				return (NULL);
			}
		}
		if ((is_cmd(sep, curr) == -1 && curr->redirection == 0) && (i == 0 || ft_strncmp(sep[i], "|", 2) == 0))
		{
			printf("%s: command not found\n", sep[i]);
			return NULL;
		}
		if (!curr->cmd)
			curr->cmd = sep[i];
		int count = 0;
		if (curr->args)
			while (curr->args[count])
				count++;
		char **new_args = malloc(sizeof(char *) * (count + 2));
		for (int j = 0; j < count; j++)
			new_args[j] = curr->args[j];
		new_args[count] = sep[i];
		new_args[count + 1] = NULL;
		free(curr->args);
		curr->args = new_args;
		i++;
	}
	return head;
}
