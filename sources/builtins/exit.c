/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/03 00:36:39 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
//FOR TESTING PORPUSE ONLY
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

//define global variable for testing
int g_exit = 0;

//fake struct just for testing
typedef struct s_msh 
{
    int random; 
} t_msh;

//fake free function just for testing
void ft_free_all(t_msh *msh) 
{
    (void)msh;
    printf("Running function to free all allocated resources\n");
}
*/

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
			|| (result == LONG_MAX / 10 && (*str - '0') > LONG_MAX % 10)) // Avoid long long overflow
		{
			*error = 1;
			return (0);
		}
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
}

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

int	execute_exit(t_msh *msh, char **args)
{
	long long	exit_status;
	int			error;

	ft_putstr_fd("exit\n", 1); //prints exit as an overall feature
	if (!args[1]) //if there's no argument ir cleans data and exits with last status nown (g_exit)
	{
		ft_free_all(msh);
		exit(g_exit);
	}
	if (!ft_isnumber(args[1])) //checks if the exit argument is a number and exits with '2' if not
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ft_free_all(msh);
		exit(2); //exit with error code 2
	}
	exit_status = ft_safe_atol(args[1], &error); //convertion with overflow error management
	if (error)
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ft_free_all(msh);
		exit(2);
	}
	if (args[2]) //more than one argument
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		g_exit = 1;
		return (1);
	}
	ft_free_all(msh);
	exit(get_exit_code(exit_status)); //enshure exit status is 0-255; 
}

/*
//Must run each test indepently and comment the others

int	main(void)
{
	t_msh msh;

//	char	*args01[] = {"exit", NULL};
//	char	*args02[] = {"exit", "42", NULL};
//	char	*args03[] = {"exit", "-42", NULL};
//	char	*args04[] = {"exit", "abc", NULL};
//	char	*args05[] = {"exit", "9999999999999999999", NULL};
//	char	*args06[] = {"exit", "abc", "xyz", NULL};
	char	*args07[] = {"exit", "256", NULL};

	printf("Test 01: No arguments\n");
	execute_exit(&msh, args01);

	printf("Test 02: Valid Number (42)\n");
	execute_exit(&msh, args02);

	printf("Test 03: Negative Number\n");
	execute_exit(&msh, args03);

	printf("Test 04: Invalid Argument\n");
	execute_exit(&msh, args04);

	printf("Test 05: Overflow\n");
	execute_exit(&msh, args05);

	printf("Test 06: More than 1 Argument\n");
	execute_exit(&msh, args06);

	printf("Test 07: Exit status greater than 255\n");
	execute_exit(&msh, args07);

	return (0);
}
*/