#include "./../minishell.h"

static int	count_words_in_segment(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_WORD)
			count++;
		else if (token->type >= T_REDIR_OUT && token->type <= T_HEREDOC)
		{
			if (token->next)
				token = token->next;
		}
		token = token->next;
	}
	return (count);
}

static void	set_command_name(t_cmd *cmd)
{
	if (cmd->args[0])
		cmd->cmd = ft_strdup(cmd->args[0]);
}

t_token	*fill_command_from_segment(t_cmd *cmd, t_token *token)
{
	int		i;
	int		word_count;
	t_token	*segment_start;

	segment_start = token;
	word_count = count_words_in_segment(segment_start);
	cmd->args = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->args)
		return (NULL);
	i = 0;
	token = process_segment_tokens(cmd, token, &i);
	cmd->args[i] = NULL;
	set_command_name(cmd);
	return (token);
}

t_token	*check_redirection_token_validity(t_token *token)
{
	if (token == NULL || token->next == NULL || token->next->type != T_WORD)
	{
		if (token)
			return (token->next);
		return (NULL);
	}
	return (token);
}
