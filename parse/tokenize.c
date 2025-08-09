/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:17:29 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 12:17:30 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	word_len(const char *s)
{
	int		i;
	char	quote_char;

	i = 0;
	quote_char = 0;
	while (s[i])
	{
		if (quote_char == 0 && (s[i] == '\'' || s[i] == '"'))
		{
			quote_char = s[i];
			i++;
			while (s[i] && s[i] != quote_char)
				i++;
			if (s[i])
				i++;
			quote_char = 0;
		}
		else if (!quote_char && ft_strchr(" \t\n|<>", s[i]))
			break ;
		else
			i++;
	}
	return (i);
}

static t_token_type	get_token_type(const char *s)
{
	if (!ft_strcmp(s, "|"))
		return (T_PIPE);
	else if (!ft_strcmp(s, ">"))
		return (T_REDIR_OUT);
	else if (!ft_strcmp(s, ">>"))
		return (T_APPEND);
	else if (!ft_strcmp(s, "<"))
		return (T_REDIR_IN);
	else if (!ft_strcmp(s, "<<"))
		return (T_HEREDOC);
	return (T_WORD);
}

static char	*handle_operator_token(t_token **head, char *input)
{
	int		len;
	char	*str;

	len = operator_len(input);
	str = ft_substr(input, 0, len);
	add_token_back(head, new_token(str, get_token_type(str), 0));
	return (input + len);
}

static char	*handle_word_token(t_token **head, char *input)
{
	int		len;
	char	*str;
	int		quote_type;

	len = word_len(input);
	if (len == 0)
		return (input);
	str = ft_substr(input, 0, len);
	quote_type = 0;
	if (str[0] == '\'')
		quote_type = 1;
	else if (str[0] == '"')
		quote_type = 2;
	add_token_back(head, new_token(str, T_WORD, quote_type));
	return (input + len);
}

t_token	*tokenize(char *input)
{
	t_token	*head;

	head = NULL;
	while (*input)
	{
		while (*input && ft_strchr(" \t\n", *input))
			input++;
		if (!*input)
			break ;
		if (operator_len(input) > 0)
			input = handle_operator_token(&head, input);
		else
			input = handle_word_token(&head, input);
	}
	return (head);
}
