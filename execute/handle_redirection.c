/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:16:03 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 14:35:08 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	handle_infile_redirection(t_redirection *redir)
{
	int	fd;

	fd = open(redir->infile, O_RDONLY);
	if (fd == -1)
	{
		print_redir_error(redir->infile);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_outfile_redirection(t_redirection *redir)
{
	int	fd;

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
	return (0);
}

static int	handle_input_redirection(t_redirection *redir)
{
	int	ret;

	if (redir->type == T_HEREDOC)
	{
		dup2(redir->heredoc_fd, STDIN_FILENO);
		close(redir->heredoc_fd);
	}
	else if (redir->infile)
	{
		ret = handle_infile_redirection(redir);
		if (ret != 0)
			return (ret);
	}
	return (0);
}

static int	handle_output_redirection(t_redirection *redir)
{
	int	ret;

	if (redir->outfile)
	{
		ret = handle_outfile_redirection(redir);
		if (ret != 0)
			return (ret);
	}
	return (0);
}

int	handle_redirections_fd(t_cmd *cmd)
{
	t_redirection	*redir;
	int				ret;

	redir = cmd->redirections;
	while (redir)
	{
		ret = handle_input_redirection(redir);
		if (ret != 0)
			return (ret);
		ret = handle_output_redirection(redir);
		if (ret != 0)
			return (ret);
		redir = redir->next;
	}
	return (0);
}
