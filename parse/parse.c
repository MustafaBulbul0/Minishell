#include "./../minishell.h"

static t_cmd		*create_cmd(void);
static t_token		*handle_redirections(t_token *token, t_cmd *cmd);
static int			count_words_in_segment(t_token *token);
static t_token		*fill_command_from_segment(t_cmd *cmd, t_token *token);

static t_cmd	*create_cmd(void)
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

static t_token	*handle_redirections(t_token *token, t_cmd *cmd)
{
	t_redirection	*redir;
	t_redirection	*tmp;
	t_token_type	redir_type;

	if (token == NULL || token->next == NULL || token->next->type != T_WORD)
	{
		if (token)
			return (token->next);
		return (NULL);
	}
	redir = create_redirection();
	if (redir == NULL)
		return (NULL);
	redir_type = token->type;
	token = token->next;
	if (redir_type == T_REDIR_IN || redir_type == T_HEREDOC)
		redir->infile = ft_strdup(token->str);
	else if (redir_type == T_REDIR_OUT || redir_type == T_APPEND)
	{
		redir->outfile = ft_strdup(token->str);
		if (redir_type == T_APPEND)
			redir->append = 1;
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

static int	count_words_in_segment(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_WORD)
			count++;
		else if (token->type >= T_REDIR_OUT && token->type <= T_HEREDOC)
		{
			if (token->next)
				token = token->next;
		}
		token = token->next;
	}
	return (count);
}

static t_token	*fill_command_from_segment(t_cmd *cmd, t_token *token)
{
	int		i;
	int		word_count;
	t_token	*segment_start;

	segment_start = token;
	word_count = count_words_in_segment(segment_start);
	cmd->args = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->args)
		return (NULL);
	i = 0;
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_WORD)
		{
			cmd->args[i++] = ft_strdup(token->str);
			token = token->next;
		}
		else if (token->type >= T_REDIR_OUT && token->type <= T_HEREDOC)
			token = handle_redirections(token, cmd);
		else
			token = token->next;
	}
	cmd->args[i] = NULL;
	if (cmd->args[0])
		cmd->cmd = ft_strdup(cmd->args[0]);
	return (token);
}

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*token_ptr;

	head = NULL;
	current = NULL;
	token_ptr = tokens;
	while (token_ptr)
	{
		if (!head)
		{
			head = create_cmd();
			current = head;
		}
		else
		{
			current->next = create_cmd();
			current = current->next;
		}
		if (!current)
			return (free_commands(head), NULL);
		token_ptr = fill_command_from_segment(current, token_ptr);
		if (!current->args)
			return (free_commands(head), NULL);
		if (token_ptr && token_ptr->type == T_PIPE)
			token_ptr = token_ptr->next;
	}
	return (head);
}
