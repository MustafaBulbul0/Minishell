#include "./../minishell.h"

void	free_commands(t_cmd *cmd_list)
{
	t_cmd			*tmp_cmd;
	t_redirection	*tmp_redir;
	int				i;

	while (cmd_list != NULL)
	{
		if (cmd_list->cmd != NULL)
			free(cmd_list->cmd);
		i = 0;
		if (cmd_list->args != NULL)
		{
			while (cmd_list->args[i] != NULL)
			{
				free(cmd_list->args[i]);
				i++;
			}
			free(cmd_list->args);
		}
		while (cmd_list->redirections != NULL)
		{
			tmp_redir = cmd_list->redirections;
			if (tmp_redir->infile != NULL)
				free(tmp_redir->infile);
			if (tmp_redir->outfile != NULL)
				free(tmp_redir->outfile);
			cmd_list->redirections = tmp_redir->next;
			free(tmp_redir);
		}
		tmp_cmd = cmd_list;
		cmd_list = cmd_list->next;
		free(tmp_cmd);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->str)
			free(tmp->str);
		free(tmp);
	}
}
