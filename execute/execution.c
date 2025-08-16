/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:48 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 13:04:17 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	should_run_parent_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd || cmd->redirections)
		return (0);
	if (ft_strcmp(cmd->cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

void	cleanup_redirections(t_redirection *redirections)
{
	t_redirection	*curr;
	t_redirection	*next;

	curr = redirections;
	while (curr)
	{
		next = curr->next;
		if (curr->infile)
			free(curr->infile);
		if (curr->outfile)
			free(curr->outfile);
		free(curr);
		curr = next;
	}
}

static void	close_heredoc_fds(t_cmd *cmd_list)
{
	t_cmd			*curr_cmd;
	t_redirection	*curr_redir;

	curr_cmd = cmd_list;
	while (curr_cmd)
	{
		curr_redir = curr_cmd->redirections;
		while (curr_redir)
		{
			if (curr_redir->type == T_HEREDOC && curr_redir->heredoc_fd > 0)
			{
				close(curr_redir->heredoc_fd);
				curr_redir->heredoc_fd = -1;
			}
			curr_redir = curr_redir->next;
		}
		curr_cmd = curr_cmd->next;
	}
}

int	ft_execute(t_envlist *env, t_cmd *cmd_list, t_token *all_tokens)
{
	if (!cmd_list)
		return (PARSE_EXECUTE_OK);
	if (preprocess_heredocs(cmd_list, env, all_tokens) != 0)
		return (PARSE_EXECUTE_OK);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	// Handle case where there's no command but has heredocs
	if (!cmd_list->cmd && cmd_list->redirections)
	{
		close_heredoc_fds(cmd_list);
		return (PARSE_EXECUTE_OK);
	}
	if (list_len(cmd_list) == 1 && is_builtin(cmd_list)
		&& should_run_parent_builtin(cmd_list))
	{
		if (ft_strcmp(cmd_list->cmd, "exit") == 0)
		{
			execute_builtin(cmd_list, env, 0);
			return (PARSE_EXECUTE_EXIT);
		}
		execute_builtin(cmd_list, env, 0);
	}
	else
		execute_pipeline(cmd_list, env, cmd_list, all_tokens);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (PARSE_EXECUTE_OK);
}
