/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:20:46 by mubulbul          #+#    #+#             */
/*   Updated: 2024/10/19 18:06:09 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t sizeDest)
{
	size_t	i;
	size_t	s_dest;
	size_t	s_src;

	i = 0;
	s_dest = ft_strlen(dest);
	s_src = ft_strlen(src);
	if (sizeDest <= s_dest)
		return (sizeDest + s_src);
	if (sizeDest == 0)
		return (s_dest);
	while (src[i] && (s_dest + i) < (sizeDest - 1))
	{
		dest[s_dest + i] = src[i];
		i++;
	}
	dest[s_dest + i] = '\0';
	return (s_dest + s_src);
}
