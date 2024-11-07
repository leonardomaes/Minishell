/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:35:44 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/11 18:11:31 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char			*dest;
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	dest = (char *)malloc(i + 1);
	if (!dest)
		return (NULL);
	while (j < i)
	{
		dest[j] = s[j];
		j++;
	}
	dest[j] = '\0';
	return (dest);
}
