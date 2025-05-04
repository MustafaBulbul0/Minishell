/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:17:57 by mubulbul          #+#    #+#             */
/*   Updated: 2024/10/16 15:17:58 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int newCh, size_t sizeStr)
{
	unsigned char	*ptr;
	unsigned char	ch;
	size_t			i;

	i = 0;
	ptr = (unsigned char *)str;
	ch = (unsigned char)newCh;
	while (i < sizeStr)
	{
		ptr[i] = ch;
		i++;
	}
	return (ptr);
}
