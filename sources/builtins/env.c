/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/08 16:37:46 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_env(char **args, char **envp)
{
	int	i;

	i = 0;
	if (!args)
		return (1);
	if (args[1]) //if there are arguments do not accept them
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd("': No such file or directory\n", 2);
		return(127); //return error code 127
	}
	while (envp && envp[i])
	{
		if (strchr(envp[i], '=')) //only print variables that contain '='
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

/*
int	main(void)
{
	char	*envp[] = {"HOME=home/user", "USER=test_user", NULL};

	printf("testing env:\n");
	execute_env(envp);
	return (0);
}
*/
