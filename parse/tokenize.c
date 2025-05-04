#include "./../minishell.h"

int	special_counter(int start, char *input);
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
	if (input[start + i] == '"')
	{
		i++;
		while (input[start + i] && input[start + i] != '"')
			i++;
		if (input[start + i] == '"')
			i++;
		return (i);
	}
	else if (input[start + i] == '\'')
	{
		i++;
		while (input[start + i] && input[start + i] != '\'')
			i++;
		if (input[start + i] == '\'')
			i++;
		return (i);
	}
	else if (input[start + i] == '>')
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
			&& input[start + i] != '|' && input[start + i] != '"'
			&& input[start + i] != '\'')
		{
			i++;
		}
		return (i);
	}
}
