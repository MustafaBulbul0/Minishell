#include "minishell.h"

int	g_last_exit = 0;

void	signal_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	process_tokens(t_token **tokens_head, t_envlist *env)
{
	t_token	*current;
	t_token	*prev;
	char	*processed_str;
	char	*temp_str;
	int		original_quote_type;

	prev = NULL;
	current = *tokens_head;
	while (current)
	{
		if (current->type == T_WORD)
		{
			if (current->quote_type != 1 && ft_strchr(current->str, '$'))
			{
				temp_str = current->str;
				processed_str = expand_variable(temp_str, env);
				free(temp_str);
				current->str = processed_str;
			}
			original_quote_type = current->quote_type;
			temp_str = current->str;
			processed_str = merge_and_strip_quotes(temp_str);
			free(temp_str);
			current->str = processed_str;
			if (*(current->str) == '\0' && original_quote_type == 0)
			{
				if (prev == NULL)
					*tokens_head = current->next;
				else
					prev->next = current->next;
				free(current->str);
				free(current);
				current = (prev ? prev->next : *tokens_head);
				continue ;
			}
		}
		prev = current;
		current = current->next;
	}
}

static void	parse_execute(char *input, t_envlist *env)
{
	t_token	*tokens;
	t_cmd	*commands;

	tokens = tokenize(input);
	if (!tokens)
		return ;
	process_tokens(&tokens, env);
	commands = parse_commands(tokens);
	if (commands)
		ft_execute(env, commands);
	free_tokens(tokens);
	free_commands(commands);
}

int	main(void)
{
	char		*input;
	t_envlist	*env;

	g_last_exit = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	env = envp_init(environ);
	while (1)
	{
		input = read_multiline_input();
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		else if (!(*input))
		{
			free(input);
			continue ;
		}
		add_history(input);
		parse_execute(input, env);
		free(input);
	}
	return (g_last_exit);
}
