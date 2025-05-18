#include "../minishell.h"

void	exit_program(int argc, t_cmd *commands)
{
	int	status;

	if (argc > 2)
	{
		write(2, "exit\n", 5);
		write(2, "minishell: exit: too many arguments", 35);
	}
	else
	{
		if (is_numeric(commands->args[1]) == 0)
		{
			write(2, "exit\n", 5);
			write(2, "minishell: exit: numeric argument required", 42);
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
