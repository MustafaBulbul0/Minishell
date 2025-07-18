#include "./../minishell.h"

static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
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

static char	*ft_join(char c, char *result, int *i)
{
	char	tmp[2];
	char	*new_str;

	tmp[0] = c;
	tmp[1] = 0;
	(*i)++;
	new_str = ft_strjoin(result, tmp);
	free (result);
	return (new_str);
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
				result = ft_join(s[i], result, &i);
			if (s[i] == quote)
				i++;
		}
		else
			result = ft_join(s[i], result, &i);
	}
	return (result);
}
