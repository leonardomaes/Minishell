/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:57:49 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/08 12:57:50 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void    print_path(char *str)
{
	DIR* dir;

	dir = opendir(str);
	if (dir == NULL)
		return ;
	struct dirent* entity;
	entity = readdir(dir);
	while (entity != NULL)
	{
		if (ft_strncmp(entity->d_name, ".", 1))
			printf("%s  ", entity->d_name);
		entity = readdir(dir);
	}
	printf("\n");
	closedir(dir);
}

