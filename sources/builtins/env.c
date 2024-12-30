/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/30 00:55:51 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	execute_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (array[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
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
