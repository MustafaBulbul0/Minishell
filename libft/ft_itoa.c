/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:03:59 by mubulbul          #+#    #+#             */
/*   Updated: 2024/10/19 16:58:12 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_num_value(int n, char *num, int last_ind)
{
	int	n_last;

	n_last = last_ind;
	if (n < 0)
	{
		num[0] = '-';
		n *= -1;
	}
	if (n == 0)
	{
		num[0] = '0';
		num[1] = '\0';
		return (num);
	}
	while (n != 0)
	{
		num[last_ind] = (n % 10) + '0';
		n /= 10;
		last_ind--;
	}
	num[n_last + 1] = '\0';
	return (num);
}

char	*ft_itoa(int n)
{
	char	*num;
	int		i;
	int		n_cpy;

	i = 0;
	n_cpy = n;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
		n_cpy = -n;
	while (n_cpy != 0)
	{
		n_cpy /= 10;
		i++;
	}
	if (n <= 0)
		i++;
	num = (char *)malloc((i + 1) * sizeof(char));
	if (!num)
		return (NULL);
	return (ft_num_value(n, num, i - 1));
}
