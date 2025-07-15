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
