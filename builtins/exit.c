#include "../minishell.h"

static int	is_valid_exit_arg(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	get_exit_status(char *arg)
{
	int	status;

	if (is_valid_exit_arg(arg) == 0)
	{
		write(2, "minishell: exit: ", 17);
		write(2, arg, ft_strlen(arg));
		write(2, ": numeric argument required\n", 28);
		status = 2;
	}
	else
	{
		status = ft_atoi(arg);
		status = (unsigned char)status;
	}
	return (status);
}

void	builtin_exit(t_cmd *commands, int is_child)
{
	int	status;
	int	argc;

	argc = 0;
	while (commands->args && commands->args[argc])
		argc++;
	if (!is_child)
		write(2, "exit\n", 5);
	if (argc > 2)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		g_last_exit = 1;
		return ;
	}
	if (argc == 1)
		status = g_last_exit;
	else
		status = get_exit_status(commands->args[1]);
	exit(status);
}
