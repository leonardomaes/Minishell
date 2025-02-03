/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/03 18:11:58 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
note to myself: evaluate to Leonardo if using ft_putstr_fd instead of printf 
might be a better strategy to allow sending the outut piped to another command. 
Also check with him what would be the best argument for this funtions, array of
strings or give it direct access to the struct. 
*/

int	execute_echo(char **args)
{
	int	no_newline;
	int	i;

	no_newline = 0;
	i = 1; //assuming that echo is passed as an argument it starts printing on arg 1

	if (args[1] && (strcmp(args[i], "-n") == 0))
	{
		no_newline = 1;
		i++;
	}
	while (args[i] && get_type(args[i], 1) != TKN_PIPE)
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
	{
		printf("\n");
	}
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
