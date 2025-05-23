#include "minishell.h"

void	signal_handler(int sig);

int	main()
{
	char		*input;
	t_token		*tokens;
	t_cmd		*commands;
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
		tokens = tokenize(input);
		commands = parse_commands(tokens);
		ft_execute(env, commands);
		free_tokens(tokens);
		free_commands(commands);
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
