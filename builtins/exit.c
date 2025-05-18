#include "../minishell.h"

void	exit_program(t_cmd *commands)
{
	int	status;
	int	argc;

	argc = 0;
	while (commands->args[argc])
		argc++;
	printf("%d\n", argc);
	if (argc > 2)
	{
		write(2, "exit\n", 5);
		write(2, "minishell: exit: too many arguments\n", 36);
	}
	else
	{
		if (is_numeric(commands->args[1]) == 0)
		{
			write(2, "exit\n", 5);
			write(2, "minishell: exit: numeric argument required\n", 43);
		}
		else
		{
			status = ft_atoi(commands->args[1]);
			if (status < 0 || status > 255)
				status = status % 256;
			exit (status);
		}
	}
}
