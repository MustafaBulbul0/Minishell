/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:16:30 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 12:16:31 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static t_redirection	*create_redirection(void)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (redir == NULL)
		return (NULL);
	redir->infile = NULL;
	redir->outfile = NULL;
	redir->append = 0;
	redir->next = NULL;
	return (redir);
}

static t_token	*get_redirection_info(t_token *token, t_redirection **redir)
{
	if (token == NULL || token->next == NULL || token->next->type != T_WORD)
	{
		if (token != NULL)
			return (token->next);
		else
			return (NULL);
	}
	*redir = create_redirection();
	if (*redir == NULL)
		return (NULL);
	(*redir)->type = token->type;
	token = token->next;
	if ((*redir)->type == T_REDIR_IN || (*redir)->type == T_HEREDOC)
		(*redir)->infile = ft_strdup(token->str);
	else if ((*redir)->type == T_REDIR_OUT || (*redir)->type == T_APPEND)
	{
		(*redir)->outfile = ft_strdup(token->str);
		if ((*redir)->type == T_APPEND)
			(*redir)->append = 1;
	}
	return (token->next);
}

static t_token	*handle_redirections(t_token *token, t_cmd *cmd)
{
	t_redirection	*new_redir;
	t_redirection	*tmp;
	t_token			*next_token;

	next_token = get_redirection_info(token, &new_redir);
	if (new_redir == NULL)
		return (NULL);
	if (cmd->redirections == NULL)
		cmd->redirections = new_redir;
	else
	{
		tmp = cmd->redirections;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_redir;
	}
	return (next_token);
}

t_token	*process_segment_tokens(t_cmd *cmd, t_token *token, int *i)
{
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_WORD)
		{
			cmd->args[(*i)++] = ft_strdup(token->str);
			token = token->next;
		}
		else if (token->type >= T_REDIR_OUT && token->type <= T_HEREDOC)
			token = handle_redirections(token, cmd);
		else
			token = token->next;
	}
	return (token);
}
