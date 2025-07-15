#include "./../minishell.h"

static int	list_len(t_cmd	*list)
{
	int	i;

	i = 0;
	while (list)
	{
		i++;
		list = list->next;
	}
	return (i);
}

void	execute_builtin(t_cmd *cmd, t_envlist *env, int is_child)
{
	if (!cmd || !cmd->cmd)
		return ;
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		builtin_env(env);
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		builtin_exit(cmd, is_child);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		builtin_cd(cmd->args, env);
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		builtin_unset(env, cmd->args);
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		builtin_export(env, cmd->args);
}

static int	should_run_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd || cmd->redirections)
		return (0);
	if (ft_strcmp(cmd->cmd, "cd") == 0
		|| ft_strcmp(cmd->cmd, "export") == 0
		|| ft_strcmp(cmd->cmd, "unset") == 0
		|| ft_strcmp(cmd->cmd, "exit") == 0)
		return (1);
	return (0);
}

void	ft_execute(t_envlist *env, t_cmd *cmd_list)
{
	if (!cmd_list || !cmd_list->cmd)
		return ;
	if (list_len(cmd_list) == 1 && is_builtin(cmd_list)
		&& should_run_parent_builtin(cmd_list))
	{
		execute_builtin(cmd_list, env, 0);
		return ;
	}
	execute_pipeline(cmd_list, env);
}
