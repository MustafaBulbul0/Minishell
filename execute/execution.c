#include "./../minishell.h"

static int	is_builtins(t_cmd *commands, t_envlist *env);

void	execute_external_command(t_cmd *cmd, t_envlist *env)
{
	char	*path;
	char	**envp;

	path = get_exec_path(cmd->cmd);
	if (!path)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, "\n", 1);
		exit(127);
	}
	envp = envlist_to_array(env);
	if (execve(path, cmd->args, envp) == -1)
	{
		perror("minishell");
		free(path);
		ft_free_split(envp);
		exit(126);
	}
}

void	ft_execute(t_envlist *env, t_cmd *cmd_list)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	t_cmd	*curr;

	in_fd = 0;
	curr = cmd_list;
	while (curr)
	{
		if (curr->next != NULL)
			pipe(pipe_fd);
		pid = fork();
		if (pid == 0)
		{
			if (in_fd != 0)
			{
				dup2(in_fd, 0);
				close(in_fd);
			}
			if (curr->next != NULL)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], 1);
				close(pipe_fd[1]);
			}
			handle_redirections_fd(curr);
			if (is_builtins(curr, env))
				exit(0);
			execute_external_command(curr, env);
			exit(1);
		}
		else
		{
			waitpid(pid, NULL, 0);
			if (in_fd != 0)
				close(in_fd);
			if (curr->next != NULL)
			{
				close(pipe_fd[1]);
				in_fd = pipe_fd[0];
			}
		}
		curr = curr->next;
	}
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
