/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:07:51 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/25 18:07:52 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void ft_init_shell(t_msh **msh, char **envp)
{
	(*msh) = malloc(sizeof(t_msh));
	if (!*msh)
		return ;
	(*msh)->envp = ft_get_env(envp);
	(*msh)->cmd_paths = ft_split(ft_get_path((*msh)->envp), ':');
}
