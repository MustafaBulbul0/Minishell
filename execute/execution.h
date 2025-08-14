/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa <mustafa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:18:47 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/14 21:47:27 by mustafa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

char	**process_heredocs_in_cmd(t_cmd *curr_cmd, char **tmp_files, int *i);
int		handle_redirections_fd(t_cmd *cmd);

#endif