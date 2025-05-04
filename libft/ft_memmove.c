/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:13:18 by mustafa           #+#    #+#             */
/*   Updated: 2024/10/19 11:17:00 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*n_dest;
	const unsigned char	*n_src;
	size_t				i;

	n_dest = (unsigned char *)dest;
	n_src = (const unsigned char *)src;
	if (n_dest == n_src || n == 0)
		return (dest);
	if (n_dest > n_src)
	{
		while (n--)
			n_dest[n] = n_src[n];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			n_dest[i] = n_src[i];
			i++;
		}
	}
	return (dest);
}
