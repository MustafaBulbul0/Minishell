#include "minishell.h"

void	signal_handler(int sig);

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_cmd		*commands;
	t_envlist	*envlist;

	(void)argv;
	(void)argc;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
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
		//execute
		if (ft_strcmp(commands->args[0], "echo") == 0)
			write_line(commands);
		if (ft_strcmp(commands->args[0], "env") == 0)
			environment(&envlist, envp);
		if (ft_strcmp(commands->args[0], "exit") == 0)
			exit_program(commands);
		if (ft_strcmp(commands->args[0], "pwd") == 0)
			print_location();
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
