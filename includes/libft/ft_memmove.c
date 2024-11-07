/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:40:51 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/11 18:15:51 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned int		i;
	char				*ptr1;
	const char			*ptr2;

	if (!dest && !src)
		return (NULL);
	i = n;
	ptr1 = (char *)dest;
	ptr2 = (char *)src;
	if (ptr2 < ptr1)
	{
		while (i-- > 0)
			ptr1[i] = ptr2[i];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			ptr1[i] = ptr2[i];
			i++;
		}
	}
	return (dest);
}
