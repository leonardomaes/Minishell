/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/20 00:54:34 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_isnumber(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str) //empty string or NULL pointer
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9') //check non-digit
			return (0);
		i++;
	}
	return (1);
}

long long	ft_atol(const char *str, int *error)
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
		if (result > LONG_MAX - (*str - '0') / 10) // Avoid overflow
		{
			*error = 1;
			return (0);
		}
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
}


int	execute_exit(t_msh *msh, char **args)
{
	int	exit_status;
	int	error;

	printf("exit\n"); //prints exit as an overall feature
	if (!args[1]) //if there's no argument ir cleans data and exits with last status nown (g_exit)
	{
		ft_free_all(msh);
		exit(g_exit);
	}

	if (!ft_isnumber(args[1])) //checks if the exit argument is a number and exits with '2' if not
	{
		printf("-bash: exit: %s: numeric argument required\n", args[1]);
		ft_free_all(msh);
		exit(2); //exit with error code 2
	}
	exit_status = ft_safe_atol(args[1], &error); //convertion with overflow error management
	if (error)
	{
		printf("-bash: exit: %s: numeric argument required\n", args[1]);
		ft_free_all(msh);
		exit(2);
	}
	if (args[2]) //more than one argument
	{
		printf("-bash: exit: too many arguments");
		g_exit = 1;
		return ;
	}
	ft_free_all(msh);
	exit((int)(exit_status %  256)); //enshure exit status is 0-255; 
}

