/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_hex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 01:55:37 by lmaes             #+#    #+#             */
/*   Updated: 2024/09/30 02:10:18 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_hex(char c)
{
	if (c == 'a' || c == 'A')
		return (10);
	if (c == 'b' || c == 'B')
		return (11);
	if (c == 'c' || c == 'C')
		return (12);
	if (c == 'd' || c == 'D')
		return (13);
	if (c == 'e' || c == 'E')
		return (14);
	if (c == 'f' || c == 'F')
		return (15);
	return (c - 48);
}

int	ft_atoi_hex(char *str)
{
	int	i;
	int	num;

	num = 0;
	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		num *= 16;
		num = num + is_hex(str[i]);
		i++;
	}
	return (num);
}
