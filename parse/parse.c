#include "./../minishell.h"

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_redirection	*create_redirection(void)
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

t_token	*handle_redirections(t_token *token, t_cmd *cmd)
{
	t_redirection	*redir;
	t_redirection	*tmp;

	redir = create_redirection();
	if (redir == NULL)
		return (NULL);
	if ((token->type == T_REDIR_IN || token->type == T_HEREDOC)
		&& token->next != NULL && token->next->type == T_WORD)
	{
		token = token->next;
		redir->infile = ft_strdup(token->str);
	}
	else if ((token->type == T_REDIR_OUT || token->type == T_APPEND)
		&& token->next != NULL && token->next->type == T_WORD)
	{
		token = token->next;
		redir->outfile = ft_strdup(token->str);
		redir->append = (token->type == T_APPEND);
	}
	if (cmd->redirections == NULL)
		cmd->redirections = redir;
	else
	{
		tmp = cmd->redirections;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = redir;
	}
	return (token->next);
}

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token != NULL && token->type == T_WORD)
	{
		count++;
		token = token->next;
	}
	return (count);
}

t_token	*parse_simple_cmd(t_token *token, t_cmd *cmd)
{
	int		i;
	int		arg_count;

	arg_count = count_args(token);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (cmd->args == NULL)
		return (NULL);
	i = 0;
	while (i < arg_count)
	{
		cmd->args[i] = ft_strdup(token->str);
		token = token->next;
		i++;
	}
	cmd->args[i] = NULL;
	if (arg_count > 0)
		cmd->cmd = ft_strdup(cmd->args[0]);
	return (token);
}

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current;
	t_cmd	*tmp;
	t_cmd	*prev;
	t_token	*token;

	if (tokens == NULL)
		return (NULL);
	cmd_list = create_cmd();
	current = cmd_list;
	token = tokens;
	while (token != NULL)
	{
		if (token->type == T_WORD)
			token = parse_simple_cmd(token, current);
		else if (token->type == T_REDIR_IN || token->type == T_REDIR_OUT
			|| token->type == T_APPEND || token->type == T_HEREDOC)
			token = handle_redirections(token, current);
		else if (token->type == T_PIPE)
		{
			current->next = create_cmd();
			current = current->next;
			token = token->next;
		}
		else
			token = token->next;
	}
	tmp = cmd_list;
	prev = NULL;
	while (tmp != NULL)
	{
		if (tmp->cmd == NULL && tmp->redirections == NULL)
		{
			if (prev != NULL)
			{
				prev->next = tmp->next;
				free(tmp);
				tmp = prev->next;
			}
			else
			{
				cmd_list = tmp->next;
				free(tmp);
				tmp = cmd_list;
			}
		}
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
	return (cmd_list);
}
