#include "../minishell.h"

int	unclosed_quotes(const char *s);

char	*read_multiline_input(void)
{
	char	*line;
	char	*temp;
	char	*input;

	line = NULL;
	temp = NULL;
	input = readline("minishell> ");
	if (!input)
		return (NULL);
	while (unclosed_quotes(input))
	{
		line = readline("quote> ");
		if (!line)
			break ;
		temp = input;
		input = malloc(ft_strlen(temp) + ft_strlen(line) + 2);
		if (!input)
			return (NULL);
		input = ft_strjoin(temp, "\n");
		input = ft_strjoin(input, line);
		free(temp);
		free(line);
	}
	return (input);
}

int	unclosed_quotes(const char *s)
{
	int	s_quote;
	int	d_quote;

	s_quote = 0;
	d_quote = 0;
	while (*s)
	{
		if (*s == '\'' && d_quote % 2 == 0)
			s_quote++;
		else if (*s == '"' && s_quote % 2 == 0)
			d_quote++;
		s++;
	}
	return (s_quote % 2 != 0 || d_quote % 2 != 0);
}
