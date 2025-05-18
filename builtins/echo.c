#include "../minishell.h"

void	write_line(t_cmd *command)
{
	char	*cmd;
	char	*nl;
	int		i;

	cmd = "echo";
	nl = "-n";
	if (command->args[0] && command->args[1]
		&& ft_strcmp(command->args[0], cmd) == 0
		&& ft_strcmp(command->args[1], nl) == 0)
	{
		i = 2;
		while (command->args[i])
		{
			if (command->args[i + 1] == NULL)
			{
				printf("%s", command->args[i]);
				i++;
			}
			else
			{
				printf("%s ", command->args[i]);
				i++;
			}
		}
	}
	else if (ft_strcmp(command->args[0], cmd) == 0)
	{
		i = 1;
		while (command->args[i])
		{
			if (command->args[i + 1] == NULL)
			{
				printf("%s", command->args[i]);
				i++;
			}
			else
			{
				printf("%s ", command->args[i]);
				i++;
			}
		}
		printf("\n");
	}
}
