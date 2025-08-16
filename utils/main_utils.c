/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:37:23 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 11:39:39 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	signal_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_last_exit = 130;
}

int	input_control(char *input)
{
	if (!input)
	{
		printf("exit\n");
		g_last_exit = 1;
		return (1);
	}
	else if (!(*input))
	{
		free(input);
		return (2);
	}
	return (0);
}

void    signals(int argc, char **argv)
{
    signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	(void)argc;
	(void)argv;
}