/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:07:51 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/07 18:06:20 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


void ft_init_shell(t_msh **msh, char **envp)
{
	g_exit = 0;
	set_signal(SHELL_MODE, NULL);
	(*msh) = malloc(sizeof(t_msh));
	if (!*msh)
		return ;
	(*msh)->envp = ft_get_env(envp);
}
