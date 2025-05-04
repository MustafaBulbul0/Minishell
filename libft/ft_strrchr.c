/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa <mustafa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:16:56 by mustafa           #+#    #+#             */
/*   Updated: 2024/10/18 13:17:00 by mustafa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int ch)
{
	int				i;
	unsigned char	new_ch;

	i = ft_strlen(str);
	new_ch = (unsigned char)ch;
	while (i >= 0)
	{
		if (str[i] == new_ch)
			return ((char *)&str[i]);
		i--;
	}
	return (NULL);
}
