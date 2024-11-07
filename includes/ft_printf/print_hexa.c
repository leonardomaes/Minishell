/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:32:20 by lmaes             #+#    #+#             */
/*   Updated: 2024/05/17 11:06:32 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_hexa(unsigned long nbr, char format)
{
	char	*base;
	char	hexa[20];
	int		i;
	int		j;

	i = 0;
	if (nbr == 0)
	{
		if (format == 'p')
			base = "(nil)";
		else
			base = "0";
		print_str(base);
		return (ft_strlen(base));
	}
	base = print_base(format);
	while (nbr > 0)
	{
		j = nbr % 16;
		hexa[i] = base[j];
		nbr /= 16;
		i++;
	}
	hexa[i] = '\0';
	return (rever_hexa(hexa, format));
}
