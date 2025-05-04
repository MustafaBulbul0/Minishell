/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:05:52 by mubulbul          #+#    #+#             */
/*   Updated: 2024/10/16 15:05:53 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void	*content)
{
	t_list	*n_list;

	n_list = (t_list *)malloc(sizeof(*n_list));
	if (!n_list)
		return (NULL);
	n_list -> content = content;
	n_list -> next = NULL;
	return (n_list);
}
