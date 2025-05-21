#include "./../minishell.h"

static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	operator_len(const char *s)
{
	if ((s[0] == '<' || s[0] == '>') && s[1] == s[0])
		return (2);
	if (s[0] == '|' || s[0] == '<' || s[0] == '>')
		return (1);
	return (0);
}

static int	token_len(const char *s)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	if (is_operator_char(s[0]))
		return (operator_len(s));
	while (s[i])
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		else if (!quote && (s[i] == ' ' || is_operator_char(s[i])))
			break ;
		i++;
	}
	return (i);
}

char	**smart_split(const char *input)
{
	char	**tokens;
	int		j;
	int		len;

	j = 0;
	tokens = malloc(sizeof(char *) * (ft_strlen(input) + 1));
	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input == '\0')
			break ;
		len = token_len(input);
		if (len == 0)
			break ;
		tokens[j++] = ft_substr(input, 0, len);
		input += len;
	}
	tokens[j] = NULL;
	return (tokens);
}

char	*merge_and_strip_quotes(const char *s)
{
	char	*result;
	int		i;
	char	quote;

	i = 0;
	result = ft_calloc(1, 1);
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
			{
				char tmp[2] = {s[i], 0};
				char *old = result;
				result = ft_strjoin(result, tmp);
				free(old);
				i++;
			}
			if (s[i] == quote)
				i++;
		}
		else
		{
			char tmp[2] = {s[i], 0};
			char *old = result;
			result = ft_strjoin(result, tmp);
			free(old);
			i++;
		}
	}
	result = ft_strtrim(result, "\n");
	return (result);
}
