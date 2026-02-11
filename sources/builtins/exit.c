/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/15 01:07:23 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//helper function to check overflow
static int	check_overflow(long long res, char next, int sign)
{
	int	max_rem;

	if (sign == -1)
		max_rem = -(LONG_MIN % 10);
	else
		max_rem = LONG_MAX % 10;
	if (res > LONG_MAX / 10 || (res == LONG_MAX / 10 && (next - '0') > max_rem))
		return (1);
	return (0);
}

//atol function that handles long long overflow
long long	ft_safe_atol(const char *str, int *error)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	*error = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (check_overflow(result, *str, sign))
		{
			*error = 1;
			return (sign * (long long)LONG_MAX + (sign == -1));
		}
		result = result * 10 + (*str++ - '0');
	}
	return (result * sign);
}

//get the exit code insuring its between 0-255
int	get_exit_code(long long n)
{
	unsigned char	result;

	if (n < 0)
		result = 256 - ((-n) % 256);
	else
		result = n % 256;
	g_exit = result;
	return (n % 256);
}

//main funtion do deal with env builtin
int	execute_exit(t_msh *msh, char **args)
{
	long long	exit_status;
	int			error;

	ft_putstr_fd("exit\n", 1);
	if (!args[1] || ft_strcmp(args[1], "--") == 0)
	{
		ft_free_all(msh);
		exit(g_exit);
	}
	if (!ft_isnumber(args[1]))
		ft_exit(msh, 2, ": numeric argument required\n",
			ft_strjoin("exit: ", args[1]));
	exit_status = ft_safe_atol(args[1], &error);
	if (error)
		ft_exit(msh, 2, ": numeric argument required\n",
			ft_strjoin("exit: ", args[1]));
	if (args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		g_exit = 1;
		return (1);
	}
	ft_free_all(msh);
	exit(get_exit_code(exit_status));
}
