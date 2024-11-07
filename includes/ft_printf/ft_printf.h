/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:56:48 by lmaes             #+#    #+#             */
/*   Updated: 2024/05/21 09:56:32 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include "../libft/libft.h"

int		ft_printf(const char *format, ...);
int		print_char(char c);
int		print_nbr(int nbr);
int		print_str(char *str);
int		print_hexa(unsigned long nbr, char format);
int		rever_hexa(char *src1, char format);
int		print_unsigned(unsigned int nbr);
char	*print_base(char format);

#endif
