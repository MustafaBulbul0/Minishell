#include "./../minishell.h"

static char	*expand_tilde(char *str, t_envlist *env)
{
	char	*home_path;
	char	*expanded_path;

	if (str[0] != '~')
		return (ft_strdup(str));
	home_path = find_value("HOME", env);
	if (!home_path)
	{
		if (ft_strcmp(str, "~") == 0)
			return (ft_strdup("~"));
		else
			return (ft_strdup(str));
	}
	if (ft_strcmp(str, "~") == 0)
	{
		expanded_path = ft_strdup(home_path);
		free(home_path);
		return (expanded_path);
	}
	expanded_path = ft_strjoin(home_path, str + 1);
	free(home_path);
	return (expanded_path);
}

static void	perform_expansion_and_stripping(t_token *token, t_envlist *env)
{
	char	*temp_str;
	char	*processed_str;

	temp_str = expand_tilde(token->str, env);
	free(token->str);
	token->str = temp_str;
	if (token->quote_type != 1 && ft_strchr(token->str, '$'))
	{
		temp_str = token->str;
		processed_str = expand_variable(temp_str, env);
		free(temp_str);
		token->str = processed_str;
	}
	temp_str = token->str;
	processed_str = merge_and_strip_quotes(temp_str);
	free(temp_str);
	token->str = processed_str;
}

static void	remove_current_token(t_token **tokens_head, t_token *prev,
	t_token *current)
{
	if (prev == NULL)
		*tokens_head = current->next;
	else
		prev->next = current->next;
	free(current->str);
	free(current);
}

static int	handle_word_token_logic(t_token *current, t_envlist *env,
	t_token **tokens_head, t_token *prev)
{
	int	original_quote_type;

	original_quote_type = current->quote_type;
	perform_expansion_and_stripping(current, env);
	if (*(current->str) == '\0' && original_quote_type == 0)
	{
		remove_current_token(tokens_head, prev, current);
		return (1);
	}
	return (0);
}

void	process_tokens(t_token **tokens_head, t_envlist *env)
{
	t_token	*current;
	t_token	*prev;

	prev = NULL;
	current = *tokens_head;
	while (current)
	{
		if (current->type == T_WORD)
		{
			if (handle_word_token_logic(current, env, tokens_head, prev))
			{
				if (prev != NULL)
					current = prev->next;
				else
					current = *tokens_head;
				continue ;
			}
		}
		prev = current;
		current = current->next;
	}
}

void	parse_execute(char *input, t_envlist *env)
{
	t_token	*tokens;
	t_cmd	*commands;

	tokens = tokenize(input);
	if (!tokens)
		return ;
	if (check_syntax(tokens))
	{
		free_tokens(tokens);
		return ;
	}
	process_tokens(&tokens, env);
	commands = parse_commands(tokens);
	if (commands)
		ft_execute(env, commands);
	free_tokens(tokens);
	free_commands(commands);
}
