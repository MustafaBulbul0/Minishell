/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:18:47 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 12:18:48 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

void	execute_pipeline(t_cmd *cmd, t_envlist *env);
char	**process_heredocs_in_cmd(t_cmd *curr_cmd, char **tmp_files, int *i);
int		handle_redirections_fd(t_cmd *cmd);
void	execute_external_command(t_cmd *cmd, t_envlist *env);

#endif