#include "./../minishell.h"

t_token_type	get_token_type(const char *s)
{
	if (!ft_strcmp(s, "|"))
		return (T_PIPE);
	else if (!ft_strcmp(s, ">"))
		return (T_REDIR_OUT);
	else if (!ft_strcmp(s, ">>"))
		return (T_APPEND);
	else if (!ft_strcmp(s, "<"))
		return (T_REDIR_IN);
	else if (!ft_strcmp(s, "<<"))
		return (T_HEREDOC);
	else if (!ft_strcmp(s, "&&"))
		return (T_AND);
	else if (!ft_strcmp(s, "||"))
		return (T_OR);
	else if (!ft_strcmp(s, "("))
		return (T_OPEN_PAREN);
	else if (!ft_strcmp(s, ")"))
		return (T_CLOSE_PAREN);
	return (T_WORD);
}

t_token	*tokenize(char *input)
{
	t_token	*head;
	char	**split;
	int		i;

	i = -1;
	head = NULL;
	split = smart_split(input);
	while (split[++i])
	{
		t_token *new = malloc(sizeof(t_token));
		new->str = merge_and_strip_quotes(split[i]);
		new->type = get_token_type(split[i]);
		new->next = NULL;
		if (!head)
			head = new;
		else
		{
			t_token *tmp = head;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
	free(split);
	return (head);
}
