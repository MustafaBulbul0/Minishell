/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:06:03 by mubulbul          #+#    #+#             */
/*   Updated: 2024/10/16 15:06:04 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t num)
{
	size_t			i;
	unsigned char	new_c;
	unsigned char	*ptr;

	i = 0;
	new_c = (unsigned char)c;
	ptr = (unsigned char *)str;
	while (i < num)
	{
		if (ptr[i] == new_c)
			return (&ptr[i]);
		i++;
	}
	return (NULL);
}
