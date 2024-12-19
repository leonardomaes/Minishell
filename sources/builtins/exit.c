/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/25 19:03:46 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int ft_isnumber(const char *str)
{
    int i;

    i = 0;
    if (!str || !*str) //empty string or pointer
        return (0);
    if (str[i] == '+' || str[i] == '-') 
        i++;
    while (str[i])
    {
        if(stri[i] < '0' || str[i] > '9') //or use libft digit function
            return (0);
        i++; 
    }
    return (1);
}

long


int	execute_exit(t_msh *msh, char **args)
{
    int exit_status;
    
	printf("exit\n"); //prints exit as an overall feature

	if (!args[1]) //if there's no argument ir cleans data and exits with last status nown (g_exit)
	{
        ft_free_all(msh);
		exit(g_exit);
	}

    if (!ft_isnumber(args[1])) //checks if the exit argument is a number and exits with '2' if not
    {
        printf("exit: %s: numeric argument required\n", args[1]);
        ft_free_all(msh);
        exit(2); //exit with error code 2
    }
    exit_status = ft_safe_atol(args[1]);
}

