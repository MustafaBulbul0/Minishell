/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:16:34 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 15:17:37 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	unclosed_quotes(const char *s);

char	*read_multiline_input(void)
{
	char	*temp;

	temp = readline("\001\033[1;32m\002minishell> \001\033[0m\002");
	if (!temp)
		return (NULL);
	if (unclosed_quotes(temp))
	{
		write(2, "minishell: syntax error: unclosed quotes\n", 42);
		free(temp);
		return (ft_strdup(""));
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
