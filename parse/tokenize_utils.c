/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:17:25 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 12:17:26 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

t_token	*new_token(char *str, t_token_type type, int quote_type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = str;
	token->type = type;
	token->quote_type = quote_type;
	token->next = NULL;
	return (token);
}

void	add_token_back(t_token **head, t_token *new)
{
	t_token	*current;

	if (!head || !new)
		return ;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
}

int	operator_len(const char *s)
{
	if (!s)
		return (0);
	if ((s[0] == '<' || s[0] == '>') && s[1] == s[0])
		return (2);
	if (s[0] == '|' || s[0] == '<' || s[0] == '>')
		return (1);
	return (0);
}
