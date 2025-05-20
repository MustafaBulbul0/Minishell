#include "./../minishell.h"

static int	is_builtins(t_cmd *commands, t_envlist *env);

void	ft_execute(t_envlist *env, t_cmd *command)
{
	if (is_builtins(command, env) || ft_access(command->cmd))
		return ;
}

static int	is_builtins(t_cmd *commands, t_envlist *env)
{
	if (ft_strcmp(commands->cmd, "echo") == 0)
		write_line(commands);
	else if (ft_strcmp(commands->cmd, "env") == 0)
		ft_envp(env);
	else if (ft_strcmp(commands->cmd, "exit") == 0)
		exit_program(commands);
	else if (ft_strcmp(commands->cmd, "pwd") == 0)
		print_location();
	else if (ft_strcmp(commands->cmd, "cd") == 0)
		builtin_cd(commands->args);
	else if (ft_strcmp(commands->cmd, "unset") == 0)
		ft_unset(env, commands->args);
	else if (ft_strcmp(commands->cmd, "export") == 0)
		ft_export(env, commands->args);
	return (ft_strcmp(commands->cmd, "echo") == 0
		|| ft_strcmp(commands->cmd, "env") == 0
		|| ft_strcmp(commands->cmd, "exit") == 0
		|| ft_strcmp(commands->cmd, "pwd") == 0
		|| ft_strcmp(commands->cmd, "cd") == 0
		|| ft_strcmp(commands->cmd, "unset") == 0
		|| ft_strcmp(commands->cmd, "export") == 0);
}
