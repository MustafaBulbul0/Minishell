#include "./../minishell.h"

static int	is_redir(t_token_type type)
{
	return (type == T_REDIR_OUT || type == T_REDIR_IN || type == T_APPEND
		|| type == T_HEREDOC);
}

static int	is_operator(t_token_type type)
{
	return (is_redir(type) || type == T_PIPE);
}

int	check_first_token_syntax(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == T_PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		g_last_exit = 2;
		return (1);
	}
	return (0);
}

static int	handle_operator_syntax_error(t_token *next_token)
{
	if (!next_token)
	{
		write(2,
			"minishell: syntax error near unexpected token `newline'\n", 56);
	}
	else
	{
		write(2, "minishell: syntax error near unexpected token `", 47);
		write(2, next_token->str, ft_strlen(next_token->str));
		write(2, "'\n", 2);
	}
	g_last_exit = 2;
	return (1);
}

int	check_middle_tokens_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (!current)
		return (0);
	while (current)
	{
		if (is_operator(current->type))
		{
			if (!current->next || is_operator(current->next->type))
			{
				return (handle_operator_syntax_error(current->next));
			}
		}
		current = current->next;
	}
	return (0);
}
