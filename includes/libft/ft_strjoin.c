/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 09:25:38 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/15 09:43:16 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	str_len(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	char			*str;

	if (!s1 || !s2)
		return (NULL);
	i = str_len(s1);
	i += str_len(s2);
	str = (char *)malloc(sizeof(char) * ((i) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (*s1 != '\0')
	{
		str[i++] = *s1++;
	}
	while (*s2 != '\0')
	{
		str[i++] = *s2++;
	}
	str[i] = '\0';
	return (str);
}
