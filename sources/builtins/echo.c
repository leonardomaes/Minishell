/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/17 23:20:11 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//helper function to validade the n flag (no newline)
static int	is_valid_n_flag(const char *arg)
{
	int	i;

	i = 1;
	if (!arg || arg[0] != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

//main funtion do deal with echo builtin
int	execute_echo(t_msh *msh, char **args)
{
	int	no_newline;
	int	i;

	no_newline = 0;
	i = 1;
	while (args[1] && is_valid_n_flag(args[i]))
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
