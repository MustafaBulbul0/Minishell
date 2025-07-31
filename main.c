#include "minishell.h"

int	g_last_exit = 0;

void	signal_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_last_exit = 130;
}

static int	input_control(char *input)
{
	if (!input)
	{
		printf("exit\n");
		return (1);
	}
	else if (!(*input))
	{
		free(input);
		return (2);
	}
	return (0);
}

int	main(int argc, char **argv, char **environ)
{
	char		*input;
	t_envlist	*env;
	int			control;

	g_last_exit = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	(void)argc;
	(void)argv;
	env = envp_init(environ);
	while (1)
	{
		input = read_multiline_input();
		control = input_control(input);
		if (control == 1)
			break ;
		if (control == 2)
			continue ;
		add_history(input);
		parse_execute(input, env);
		free(input);
	}
	free_env(env);
	return (g_last_exit);
}
