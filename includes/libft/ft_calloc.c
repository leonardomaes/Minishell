/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:54:52 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/22 11:07:56 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	b_zero(void *s, size_t n)
{
	unsigned int	i;
	char			*c;

	i = 0;
	c = (char *)s;
	while (i < n)
	{
		c[i] = '\0';
		i++;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void			*ptr;
	size_t			i;

	i = nmemb * size;
	if ((nmemb > 2147483647 && size != 0) || (size > 2147483647 && nmemb != 0))
		return (NULL);
	ptr = malloc(i);
	if (!ptr)
		return (NULL);
	b_zero(ptr, i);
	return (ptr);
}
