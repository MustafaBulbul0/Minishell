/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:21:35 by mubulbul          #+#    #+#             */
/*   Updated: 2024/10/19 21:56:27 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	next_trim(char const *s1, char const *set)
{
	size_t	l;

	l = 0;
	while (s1[l] && ft_strchr(set, s1[l]))
		l++;
	return (l);
}

static size_t	back_trim(char const *s1, char const *set)
{
	size_t	l;

	l = ft_strlen(s1);
	while (l > 0 && ft_strchr(set, s1[l - 1]))
		l--;
	return (l);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	b_trim;
	size_t	n_trim;
	size_t	n_total_size;
	char	*ptr;

	if (!s1 || !set)
		return (NULL);
	n_trim = next_trim(s1, set);
	b_trim = back_trim(s1, set);
	n_total_size = b_trim - n_trim;
	if (n_trim >= b_trim)
		return (ft_strdup(""));
	ptr = (char *)malloc(n_total_size + 1);
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1 + n_trim, n_total_size + 1);
	return (ptr);
}
