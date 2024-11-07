/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 08:48:28 by lmaes             #+#    #+#             */
/*   Updated: 2024/05/03 14:28:02 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	str_len(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*sub;

	if (!s)
		return (NULL);
	j = 0;
	i = 0;
	if (start > str_len(s))
		len = 0;
	else if (len > (str_len(s) - start))
		len = str_len(s) - start;
	sub = malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	while (s[i] != '\0' && (j < len))
	{
		if (i >= start)
			sub[j++] = s[i];
		i++;
	}
	sub[j] = '\0';
	return (sub);
}
