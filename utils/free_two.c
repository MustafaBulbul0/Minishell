/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_two.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:00:25 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 13:24:12 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void    free_t_all(t_all *all)
{
    if (!all)
        return;
    if (all->all_commands)
        free_commands(all->all_commands);
    if (all->env)
        free_env(all->env);
    if (all->all_tokens)
        free_tokens(all->all_tokens);
    free(all);
    all = NULL;
}
