/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/11 18:46:03 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//check if string contains a valid number
int	ft_isnumber(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
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
		if ((result > LONG_MAX / 10)
			|| (result == LONG_MAX / 10 && (*str - '0') > LONG_MAX % 10))
		{
			return (*error = 1, 0);
		}
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
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
