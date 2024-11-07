/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:23:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/04/15 07:55:21 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	int		i;

	i = 0;
	dest_len = 0;
	src_len = 0;
	while (src[src_len] != '\0')
		src_len++;
	if (size < 1)
		return (src_len);
	while ((dst[dest_len] != '\0') && (dest_len < size))
		dest_len++;
	if (size <= dest_len)
		return (dest_len + src_len);
	while ((src[i] != '\0') && ((dest_len + i + 1) < size))
	{
		dst[dest_len + i] = src[i];
		i++;
	}
	if (dest_len + i < size)
	{
		dst[dest_len + i] = '\0';
	}
	return (dest_len + src_len);
}
