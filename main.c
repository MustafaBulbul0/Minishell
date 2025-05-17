#include "minishell.h"

void	signal_handler(int sig);

int	main(void)
{
	char	*input;
//	t_cmd	*command;
	char 	**sep;

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
			continue ;
		add_history(input);
//		command = token_separate(input);
		sep = token_separate(input);

		for (int i = 0; sep[i] ;i++)
			printf("%s\n", sep[i]);

	
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
