#include "minishell.h"

void	signal_handler(int sig);
static void	parse_execute(char *input, t_envlist *env);

int	main()
{
	char		*input;
	t_envlist	*env;

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
		parse_execute(input, env);
		add_history(input);
		free(input);
	}
	return (0);
}

void	signal_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	parse_execute(char *input, t_envlist *env)
{
	t_token		*tokens;
	t_cmd		*commands;


	tokens = tokenize(input);
	commands = parse_commands(tokens);
	ft_execute(env, commands);
	free_tokens(tokens);
	free_commands(commands);
}
