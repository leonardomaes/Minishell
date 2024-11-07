/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:10:43 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/30 11:18:19 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *contend)
{
	t_list	*x;

	x = (t_list *)malloc(sizeof(t_list));
	if (!x)
		return (NULL);
	x->content = (void *)contend;
	x->next = NULL;
	return (x);
}
