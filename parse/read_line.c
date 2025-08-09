/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esir <esir@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:16:34 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 16:29:34 by esir             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	unclosed_quotes(const char *s);

char	*read_multiline_input(void)
{
	char	*line;
	char	*temp;
	char	*joined;

	temp = readline("\001\033[1;32m\002minishell> \001\033[0m\002");
	if (!temp)
		return (NULL);
	while (unclosed_quotes(temp))
	{
		line = readline("quote> ");
		if (!line)
			break ;
		joined = ft_strjoin(temp, "\n");
		free(temp);
		if (!joined)
			return (NULL);
		temp = ft_strjoin(joined, line);
		free(joined);
		free(line);
		if (!temp)
			return (NULL);
	}
	return (temp);
}

static int	unclosed_quotes(const char *s)
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
