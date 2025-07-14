#include "./../minishell.h"

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
		builtin_cd(cmd->args, env);
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		ft_unset(env, cmd->args);
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		ft_export(env, cmd->args);
}

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

static pid_t	execute_pipe_segment(t_cmd *cmd, t_envlist *env,
				int in_fd, int *pipe_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		g_last_exit = 1;
		return (-1);
	}
	if (pid == 0)
	{
		if (in_fd != STDIN_FILENO)
		{
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}
		if (pipe_fd)
		{
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
		}
		handle_redirections_fd(cmd);

		if (is_builtin(cmd))
		{
			execute_builtin(cmd, env);
			exit(g_last_exit);
		}
		else
		{
			execute_external_command(cmd, env);
		}
	}
	return (pid);
}

static void	wait_for_children(pid_t *pids, int num_cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			g_last_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_last_exit = 128 + WTERMSIG(status);
		i++;
	}
}

static void	handle_single_builtin(t_cmd *cmd, t_envlist *env)
{
	if (ft_strcmp(cmd->cmd, "exit") == 0)
	{
		exit_program(cmd);
	}
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		builtin_cd(cmd->args, env);
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		ft_export(env, cmd->args);
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		ft_unset(env, cmd->args);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		ft_envp(env);
	else if (ft_strcmp(cmd->cmd, "echo") == 0)
		write_line(cmd);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		print_location();
}

void	ft_execute(t_envlist *env, t_cmd *cmd_list)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	*pids;
	int		num_cmds;
	t_cmd	*curr;
	int		i;

	num_cmds = list_len(cmd_list);
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
	{
		perror("minishell: malloc");
		g_last_exit = 1;
		return ;
	}
	in_fd = STDIN_FILENO;
	curr = cmd_list;
	i = 0;

	if (num_cmds == 1 && is_builtin(curr) && !curr->redirections)
	{
		handle_single_builtin(curr, env);
		free(pids);
		return ;
	}

	while (curr)
	{
		if (curr->next)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("minishell: pipe");
				g_last_exit = 1;
				free(pids);
				return ;
			}
		}

		pids[i] = execute_pipe_segment(curr, env, in_fd,
			curr->next ? pipe_fd : NULL);
		if (pids[i] == -1)
		{
			if (curr->next)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			if (in_fd != STDIN_FILENO)
				close(in_fd);
			free(pids);
			return ;
		}

		if (in_fd != STDIN_FILENO)
			close(in_fd);

		if (curr->next)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}
		curr = curr->next;
		i++;
	}
	wait_for_children(pids, num_cmds);
	free(pids);
}
