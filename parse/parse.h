#ifndef PARSE_H
# define PARSE_H

typedef struct s_token	t_token;

void	add_token_back(t_token **head, t_token *new);
int		operator_len(const char *s);
t_token	*fill_command_from_segment(t_cmd *cmd, t_token *token);
t_token	*check_redirection_token_validity(t_token *token);
t_token	*process_segment_tokens(t_cmd *cmd, t_token *token, int *i);
int		check_syntax(t_token *tokens);
int		check_middle_tokens_syntax(t_token *tokens);
int		check_first_token_syntax(t_token *tokens);

#endif