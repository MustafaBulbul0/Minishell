#include "./../minishell.h"

int	special_counter(int start, char *input);
char	*edit_sep(char *sep);
int	is_qote(char *input, int start, int i);
char	**token_separate(char *input)
{
	char	**sep;
	int		start;
	int		len;
	int		i;

	input = ft_strtrim(input, " ");
	sep = malloc(sizeof(char *) * (ft_strlen(input) + 1));
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
	return (sep);
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
