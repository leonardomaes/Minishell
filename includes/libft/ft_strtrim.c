/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 09:37:21 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/23 10:55:51 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	str_len(char const *str)
{
	unsigned int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

static char	*str_chr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
		{
			return ((char *)s);
		}
		s++;
	}
	if (c == '\0')
	{
		return ((char *)s);
	}
	return (NULL);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char			*str;
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (str_chr(set, s1[i]) && s1[i])
		i++;
	j = str_len(s1);
	while ((j > i) && str_chr(set, s1[j - 1]))
		j--;
	str = (char *)malloc((sizeof(char)) * (j - i + 1));
	if (!str)
		return (NULL);
	k = 0;
	while (i < j)
		str[k++] = s1[i++];
	str[k] = '\0';
	return (str);
}
