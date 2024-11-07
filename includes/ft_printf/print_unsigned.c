/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:52:11 by lmaes             #+#    #+#             */
/*   Updated: 2024/05/17 11:58:05 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_unsigned(unsigned int nbr)
{
	int		len;
	char	*num;

	len = 0;
	if (nbr == 0)
		len += write(1, "0", 1);
	else
	{
		num = ft_uitoa(nbr);
		len += print_str(num);
		free(num);
	}
	return (len);
}
