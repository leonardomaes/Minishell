/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strextract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 09:37:21 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/23 10:55:51 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strextract(const char *str, char delimiter)
{
	char	*start;
	char	*end;
	char	*result;
	size_t	len;

	start = ft_strchr(str, delimiter);
	if (!start)
		return (NULL);
	start++;
	end = ft_strchr(start, delimiter);
	if (!end)
		return (NULL);
	len = end - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len);
	result[len] = '\0';
	return (result);
}
