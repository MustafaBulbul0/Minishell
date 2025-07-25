#include "./../minishell.h"

static int	list_len(t_cmd *list)
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

static char	**preprocess_heredocs(t_cmd *cmd_list)
{
	t_cmd	*curr_cmd;
	char	**tmp_files;
	int		i;

	tmp_files = malloc(sizeof(char *) * (list_len(cmd_list) * 10 + 1));
	if (!tmp_files)
		return (NULL);
	i = 0;
	curr_cmd = cmd_list;
	while (curr_cmd)
	{
		if (!process_heredocs_in_cmd(curr_cmd, tmp_files, &i))
			return (NULL);
		curr_cmd = curr_cmd->next;
	}
	tmp_files[i] = NULL;
	return (tmp_files);
}

static int	should_run_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd || cmd->redirections)
	{
		return (0);
	}
	if (ft_strcmp(cmd->cmd, "cd") == 0)
	{
		return (1);
	}
	else if (ft_strcmp(cmd->cmd, "export") == 0)
	{
		return (1);
	}
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
	{
		return (1);
	}
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

static void	cleanup_heredoc_files(char **heredoc_files)
{
	int	i;

	if (heredoc_files)
	{
		i = 0;
		while (heredoc_files[i])
		{
			unlink(heredoc_files[i]);
			i++;
		}
		ft_free_split(heredoc_files);
	}
}

void	ft_execute(t_envlist *env, t_cmd *cmd_list)
{
	char	**heredoc_files;

	if (!cmd_list)
	{
		return ;
	}
	heredoc_files = preprocess_heredocs(cmd_list);
	if (!heredoc_files && errno != 0)
	{
		g_last_exit = 1;
		return ;
	}
	if (list_len(cmd_list) == 1 && is_builtin(cmd_list)
		&& should_run_parent_builtin(cmd_list))
	{
		execute_builtin(cmd_list, env, 0);
	}
	else
	{
		execute_pipeline(cmd_list, env);
	}
	cleanup_heredoc_files(heredoc_files);
}
