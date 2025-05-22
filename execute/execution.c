#include "./../minishell.h"

static void	execute_builtin(t_cmd *cmd, t_envlist *env);

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
		if (!curr->next && is_builtin(curr))
		{
			handle_redirections_fd(curr);
			execute_builtin(curr, env);
		}
		else
		{
			if (curr->next)
				pipe(pipe_fd);
			pid = fork();
			if (pid == 0)
			{
				if (in_fd != 0)
				{
					dup2(in_fd, 0);
					close(in_fd);
				}
				if (curr->next)
				{
					close(pipe_fd[0]);
					dup2(pipe_fd[1], 1);
					close(pipe_fd[1]);
				}
				handle_redirections_fd(curr);
				if (is_builtin(curr))
					exit(0);
				execute_external_command(curr, env);
				exit(1);
			}
			else
			{
				waitpid(pid, NULL, 0);
				if (in_fd != 0)
					close(in_fd);
				if (curr->next)
				{
					close(pipe_fd[1]);
					in_fd = pipe_fd[0];
				}
			}
		}
		curr = curr->next;
	}
}

static void	execute_builtin(t_cmd *cmd, t_envlist *env)
{
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		write_line(cmd);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		ft_envp(env);
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		exit_program(cmd);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		print_location();
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		builtin_cd(cmd->args);
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		ft_unset(env, cmd->args);
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		ft_export(env, cmd->args);
}