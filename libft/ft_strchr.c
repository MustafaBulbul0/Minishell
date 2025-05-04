/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:19:40 by mubulbul          #+#    #+#             */
/*   Updated: 2024/10/19 13:29:51 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int ch)
{
	int				i;
	unsigned char	new_ch;

	i = 0;
	new_ch = (unsigned char)ch;
	while (1)
	{
		if (str[i] == new_ch)
			return ((char *)&str[i]);
		if (str[i] == '\0')
			break ;
		i++;
	}
	return (NULL);
}
