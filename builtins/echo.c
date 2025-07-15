#include "../minishell.h"

static int	delete_new_line(t_cmd *command);

void	builtin_echo(t_cmd *command)
{
	int		i;
	int		temp;

	i = delete_new_line(command);
	temp = i;
	while (command->args[i])
	{
		printf("%s", command->args[i]);
		if (command->args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (temp == 1)
		printf("\n");
	g_last_exit = 0;
}

static int	delete_new_line(t_cmd *command)
{
	char	*nl;
	int		i;
	int		j;

	nl = "-n";
	i = 1;
	j = 2;
	while (command->args[i]
		&& strncmp(command->args[i], nl, 2) == 0)
	{
		j = 2;
		while (command->args[i][j])
		{
			if ((strncmp(command->args[i] + j, "n", 1) != 0))
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}
