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

static t_redirection	*create_and_assign_redirection(t_token *token,
	t_token_type redir_type)
{
	t_redirection	*redir;

	redir = create_redirection();
	if (redir == NULL)
		return (NULL);
	if (redir_type == T_REDIR_IN || redir_type == T_HEREDOC)
		redir->infile = ft_strdup(token->str);
	else if (redir_type == T_REDIR_OUT || redir_type == T_APPEND)
	{
		redir->outfile = ft_strdup(token->str);
		if (redir_type == T_APPEND)
			redir->append = 1;
	}
	return (redir);
}

static void	add_redirection_to_cmd(t_cmd *cmd, t_redirection *redir)
{
	t_redirection	*tmp;

	if (cmd->redirections == NULL)
		cmd->redirections = redir;
	else
	{
		tmp = cmd->redirections;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = redir;
	}
}

static t_token	*handle_redirections(t_token *token, t_cmd *cmd)
{
	t_redirection	*redir;
	t_token_type	redir_type;
	t_token			*original_token;

	original_token = token;
	token = check_redirection_token_validity(token);
	if (token == NULL)
	{
		if (original_token != NULL)
			return (original_token->next);
		else
			return (NULL);
	}
	redir_type = token->type;
	token = token->next;
	redir = create_and_assign_redirection(token, redir_type);
	if (redir == NULL)
		return (NULL);
	add_redirection_to_cmd(cmd, redir);
	return (token->next);
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
