/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:31:52 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/26 14:28:01 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	i;
	size_t	w;

	w = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			w++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (w);
}

static void	free_array(char **str, unsigned int n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

static size_t	len_word(const char *s, char c)
{
	size_t	len;

	len = 0;
	while (*s && *s != c)
	{
		len++;
		s++;
	}
	return (len);
}

static char	*alloc_mem(const char *s, char c)
{
	size_t	len;
	size_t	i;
	char	*str;

	i = 0;
	len = len_word(s, c);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = *s++;
		i++;
	}
	str[len] = '\0';
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	str = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!str)
		return (NULL);
	while (*s)
	{
		if (*s != c)
		{
			str[i] = alloc_mem(s, c);
			if (str[i] == NULL)
				free_array(str, i);
			i++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	str[i] = NULL;
	return (str);
}
