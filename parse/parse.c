#include "./../minishell.h"

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

static t_cmd	*create_and_link_cmd(t_cmd **head, t_cmd **current)
{
	if (!*head)
	{
		*head = create_cmd();
		*current = *head;
	}
	else
	{
		(*current)->next = create_cmd();
		*current = (*current)->next;
	}
	return (*current);
}

static t_cmd	*parse_commands_loop(t_token *tokens,
		t_cmd *head, t_cmd *current)
{
	t_token	*token_ptr;

	token_ptr = tokens;
	while (token_ptr)
	{
		current = create_and_link_cmd(&head, &current);
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

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;

	if (tokens && tokens->type == T_PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		g_last_exit = 2;
		return (NULL);
	}
	head = NULL;
	current = NULL;
	return (parse_commands_loop(tokens, head, current));
}
