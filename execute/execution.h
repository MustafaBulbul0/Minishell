#ifndef EXECUTION_H
# define EXECUTION_H

void	execute_pipeline(t_cmd *cmd, t_envlist *env);
int		handle_redirections_fd(t_cmd *cmd);
void	execute_external_command(t_cmd *cmd, t_envlist *env);

#endif