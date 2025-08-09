/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:16:20 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/10 00:40:01 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

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

int	parse_execute(char *input, t_envlist *env)
{
	t_token	*tokens;
	t_cmd	*commands;
	int		should_exit;

	should_exit = PARSE_EXECUTE_OK;
	tokens = tokenize(input);
	if (!tokens)
		return (PARSE_EXECUTE_OK);
	if (check_syntax(tokens))
	{
		free_tokens(tokens);
		return (PARSE_EXECUTE_OK);
	}
	process_tokens(&tokens, env);
	commands = parse_commands(tokens);
	if (commands)
		should_exit = ft_execute(env, commands, tokens);
	free_tokens(tokens);
	free_commands(commands);
	return (should_exit);
}
