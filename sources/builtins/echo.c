/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/08 14:47:00 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
note to myself: evaluate to Leonardo if using ft_putstr_fd instead of printf 
might be a better strategy to allow sending the outut piped to another command. 
Also check with him what would be the best argument for this funtions, array of
strings or give it direct access to the struct. 
*/
static int	is_valid_n_flag(const char *arg)
{
	int i;

	i = 1;	
	if (!arg || arg[0] != '-')
		return (0);
	while(arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1); //returns 1 only if there are at least one 'n' in the flag
}


int	execute_echo(t_msh *msh, char **args)
{
	int	no_newline;
	int	i;

	no_newline = 0;
	i = 1; //assuming that echo is passed as an argument it starts printing on arg 1
	if (args[1] && is_valid_n_flag(args[i]))
	{
		no_newline = 1;
		i++;
	}
	while (args[i] && get_type(msh, args[i], 1) != TKN_PIPE)
	{
		printf("%s", args[i]);
		if (args[i + 1] && get_type(msh, args[i + 1], 1) != TKN_PIPE)
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}
/*
int	main(void)
{
	char *args[] = {"echo", "This", "is",  "a", "shell", NULL};
	char *args1[] = {"echo", "-n", "This", "is", "also", "a", "shell", NULL};

	execute_echo(args);
	execute_echo(args1);
	return (0);
}
*/
