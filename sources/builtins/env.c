/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/10 18:29:23 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//main funtion do deal with env builtin
int	execute_env(char **args, char **envp)
{
	int	i;

	i = 0;
	if (!args)
		return (1);
	if (args[1])
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd("': No such file or directory\n", 2);
		return (127);
	}
	while (envp && envp[i])
	{
		if (strchr(envp[i], '='))
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

