#include "./../minishell.h"

void	execute_builtin(t_cmd *cmd, t_envlist *env, int is_child);
void	execute_pipeline(t_cmd *cmd_list, t_envlist *env);
char	*handle_heredoc(t_redirection *redir, int index);
void	ft_free_split(char **arg);

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
	t_cmd			*curr_cmd;
	t_redirection	*curr_redir;
	char			**tmp_files;
	int				i;

	tmp_files = malloc(sizeof(char *) * (list_len(cmd_list) * 10 + 1));
	i = 0;
	curr_cmd = cmd_list;
	while (curr_cmd)
	{
		curr_redir = curr_cmd->redirections;
		while (curr_redir)
		{
			if (curr_redir->type == T_HEREDOC)
			{
				tmp_files[i] = handle_heredoc(curr_redir, i);
				if (!tmp_files[i])
				{
					ft_free_split(tmp_files);
					return (NULL);
				}
				free(curr_redir->infile);
				curr_redir->infile = ft_strdup(tmp_files[i]);
				curr_redir->type = T_REDIR_IN;
				i++;
			}
			curr_redir = curr_redir->next;
		}
		curr_cmd = curr_cmd->next;
	}
	tmp_files[i] = NULL;
	return (tmp_files);
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
	char	**heredoc_files;
	int		i;

	if (!cmd_list)
		return ;
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
