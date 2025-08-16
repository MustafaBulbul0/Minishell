/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:18:09 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 11:40:26 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_exit = 0;

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_envlist	*env;
	int			control;
	int			should_exit;

	g_last_exit = 0;
	signals(argc, argv);
	env = envp_init(envp);
	while (1)
	{
		input = read_multiline_input();
		control = input_control(input);
		if (control == 1)
			break ;
		if (control == 2)
			continue ;
		add_history(input);
		should_exit = parse_execute(input, env);
		free(input);
		if (should_exit == PARSE_EXECUTE_EXIT)
			break ;
	}
	free_env(env);
	return (g_last_exit);
}
