#include "minishell.h"

void	signal_handler(int sig);

int	main(void)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*commands;

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
		//excute
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
