/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 09:32:40 by lmaes             #+#    #+#             */
/*   Updated: 2024/05/15 13:06:06 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_arg(va_list argument, const char format)
{
	if (format == 'c')
		return (print_char(va_arg(argument, int)));
	else if (format == 's')
		return (print_str(va_arg(argument, char *)));
	else if (format == 'p')
		return (print_hexa(va_arg(argument, unsigned long), 'p'));
	else if (format == 'd' || format == 'i')
		return (print_nbr(va_arg(argument, int)));
	else if (format == 'u')
		return (print_unsigned(va_arg(argument, unsigned long)));
	else if (format == 'x' || format == 'X')
		return (print_hexa(va_arg(argument, unsigned int), format));
	else if (format == '%')
		return (print_char('%'));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	size_t	i;
	size_t	len;
	va_list	argument;

	i = 0;
	len = 0;
	va_start(argument, format);
	while (format[i])
	{
		if (format[i] == '%' && ft_strchr("cspdiuxX%", format[i + 1]))
		{
			len += print_arg(argument, format[i + 1]);
			i++;
		}
		else
		{
			print_char(format[i]);
			len++;
		}
		i++;
	}
	va_end(argument);
	return (len);
}
