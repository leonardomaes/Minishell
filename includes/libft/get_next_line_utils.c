/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sheila <sheila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:57:31 by shrodrig          #+#    #+#             */
/*   Updated: 2024/06/11 17:44:29 by sheila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strchr_gnl(char *str, int c)
{
	int	i;

	i = 0;
	if (!str || !c)
		return (0);
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strjoin_gnl(char *str, char *buffer)
{
	char	*newstr;
	int		i;
	int		j;

	if (!str)
	{
		str = (char *)malloc(sizeof(char) * 1);
		str[0] = '\0';
	}
	newstr = (char *)malloc(ft_strlen(str) + ft_strlen(buffer) + 1);
	if (!newstr)
		return (NULL);
	i = -1;
	j = 0;
	if (str)
		while (str[++i])
			newstr[i] = str[i];
	while (buffer[j])
		newstr[i++] = buffer[j++];
	newstr[i] = '\0';
	free (str);
	return (newstr);
}
