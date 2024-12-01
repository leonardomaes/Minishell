/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:39 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_builtin_type(char *name)
{
	if (ft_strcmp(name, "cd") == 0)				// Precisa de "path"
		return (BLT_CD);
	else if (ft_strcmp(name, "echo") == 0)
		return (BLT_ECHO);
	else if (ft_strcmp(name, "pwd") == 0)
		return (BLT_PWD);
	else if (ft_strcmp(name, "export") == 0)
		return (BLT_EXPORT);
	else if (ft_strcmp(name, "unset") == 0)
		return (BLT_UNSET);
	else if (ft_strcmp(name, "env") == 0)
		return (BLT_ENV);
	else if (ft_strcmp(name, "exit") == 0)
		return (BLT_EXIT);
	else
		return (0);
	
}

int	get_type(char *name)
{
	if (get_builtin_type(name) != 0) // Check if it is a builtin cmd
		return (get_builtin_type(name));
	else
		return (1);
}
