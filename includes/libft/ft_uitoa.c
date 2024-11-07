/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:48:53 by lmaes             #+#    #+#             */
/*   Updated: 2024/05/21 09:57:44 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*alloc_mem(unsigned int n, int *len)
{
	char	*str;

	if (n == 0)
		*len = 1;
	else
	{
		while (n != 0)
		{
			n = n / 10;
			(*len)++;
		}
	}
	str = (char *)malloc(sizeof(char) * (*len + 1));
	return (str);
}

char	*ft_uitoa(unsigned int n)
{
	char		*result;
	int			i;
	int			len;
	long		m;

	m = n;
	len = 0;
	result = alloc_mem(m, &len);
	if (result == NULL)
		return (NULL);
	i = 0;
	result[len] = '\0';
	while (len > i)
	{
		len--;
		result[len] = (m % 10) + '0';
		m /= 10;
	}
	return (result);
}
