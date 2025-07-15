#include "./../minishell.h"
#include <errno.h>

static void			execute_builtin(t_cmd *cmd, t_envlist *env, int is_child);
static void			execute_external_command(t_cmd *cmd, t_envlist *env);
static int			handle_redirections_fd(t_cmd *cmd);
static void			wait_for_children(pid_t last_pid);

static void	print_redir_error(char *filename)
{
	write(2, "minishell: ", 11);
	write(2, filename, ft_strlen(filename));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}

static int	handle_redirections_fd(t_cmd *cmd)
{
	t_redirection	*redir;
	int				fd;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->infile)
		{
			fd = open(redir->infile, O_RDONLY);
			if (fd == -1)
			{
				print_redir_error(redir->infile);
				return (1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (redir->outfile)
		{
			if (redir->append)
				fd = open(redir->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(redir->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				print_redir_error(redir->outfile);
				return (1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
	return (0);
}

static void	execute_builtin(t_cmd *cmd, t_envlist *env, int is_child)
{
	if (!cmd || !cmd->cmd)
		return ;
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		write_line(cmd);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		ft_envp(env);
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		exit_program(cmd, is_child);
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
	char		*path;
	char		**envp;
	struct stat	path_stat;
	int			saved_errno;

	if (!cmd->cmd || !*(cmd->cmd))
		exit(0);
	if (ft_strchr(cmd->cmd, '/'))
	{
		if (stat(cmd->cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		{
			write(2, "minishell: ", 11);
			write(2, cmd->cmd, ft_strlen(cmd->cmd));
			write(2, ": is a directory\n", 17);
			exit(126);
		}
		path = ft_strdup(cmd->cmd);
	}
	else
		path = get_exec_path(cmd->cmd);
	if (!path)
	{
		write(2, "minishell: ", 11);
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	envp = envlist_to_array(env);
	execve(path, cmd->args, envp);
	saved_errno = errno;
	write(2, "minishell: ", 11);
	write(2, cmd->cmd, ft_strlen(cmd->cmd));
	write(2, ": ", 2);
	write(2, strerror(saved_errno), ft_strlen(strerror(saved_errno)));
	write(2, "\n", 1);
	free(path);
	ft_free_split(envp);
	if (saved_errno == EACCES)
		exit(126);
	exit(127);
}

static void	wait_for_children(pid_t last_pid)
{
	int	status;
	int	exit_status;

	if (last_pid == -1)
		return ;
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			exit_status = 130;
		else if (WTERMSIG(status) == SIGQUIT)
			exit_status = 131;
		else
			exit_status = 128 + WTERMSIG(status);
	}
	else
		exit_status = status;
	while (wait(NULL) > 0)
		;
	g_last_exit = exit_status;
}

void	ft_execute(t_envlist *env, t_cmd *cmd_list)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	t_cmd	*curr;

	if (!cmd_list || !cmd_list->cmd)
		return ;
	if (list_len(cmd_list) == 1 && is_builtin(cmd_list)
		&& !cmd_list->redirections)
	{
		if (ft_strcmp(cmd_list->cmd, "cd") == 0
			|| ft_strcmp(cmd_list->cmd, "export") == 0
			|| ft_strcmp(cmd_list->cmd, "unset") == 0
			|| ft_strcmp(cmd_list->cmd, "exit") == 0)
		{
			execute_builtin(cmd_list, env, 0);
			return ;
		}
	}
	in_fd = STDIN_FILENO;
	curr = cmd_list;
	pid = -1;
	while (curr)
	{
		if (curr->next)
			pipe(pipe_fd);
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (curr->next)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			if (handle_redirections_fd(curr) != 0)
				exit(1);
			if (is_builtin(curr))
			{
				execute_builtin(curr, env, 1);
				exit(g_last_exit);
			}
			else
				execute_external_command(curr, env);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (curr->next)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}
		curr = curr->next;
	}
	wait_for_children(pid);
}
