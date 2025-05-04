/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:14:54 by mustafa           #+#    #+#             */
/*   Updated: 2024/10/19 17:57:48 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_words(char const *s, char c)
{
	size_t	words;

	words = 0;
	while (*s)
	{
		if ((*(s + 1) == c || *(s + 1) == '\0') && (*s != c))
			words++;
		s++;
	}
	return (words);
}

static char	*ft_allocate_word(char const *s, char c, size_t *i)
{
	int		len;
	int		k;
	char	*word;

	len = 0;
	while (s[*i] == c)
		(*i)++;
	while (s[*i + len] && s[*i + len] != c)
		len++;
	word = (char *)malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	k = 0;
	while (k < len)
	{
		word[k] = s[*i + k];
		k++;
	}
	word[len] = '\0';
	*i += len;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;
	size_t	i;
	size_t	j;
	size_t	word_count;

	i = 0;
	j = 0;
	word_count = ft_count_words(s, c);
	ptr = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!ptr)
		return (0);
	ptr[word_count] = NULL;
	while (j < word_count)
	{
		ptr[j] = ft_allocate_word(s, c, &i);
		if (!ptr[j])
			return (NULL);
		j++;
	}
	return (ptr);
}
