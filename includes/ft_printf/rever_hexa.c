/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rever_hexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:30:59 by lmaes             #+#    #+#             */
/*   Updated: 2024/05/15 13:35:24 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	rever_hexa(char *src1, char format)
{
	int		i;
	int		j;
	char	*src2;

	i = strlen(src1) - 1;
	src2 = malloc(i + 2 * sizeof(char));
	if (!src2)
		return (-1);
	j = 0;
	while (i >= 0)
	{
		src2[j] = src1[i];
		j++;
		i--;
	}
	src2[j] = '\0';
	print_str(src2);
	free(src2);
	if (format == 'p')
		j += 2;
	return (j);
}
