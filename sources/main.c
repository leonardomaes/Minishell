/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/07 12:50:02 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int main()
{
	DIR* dir = opendir(".");
	if (dir == NULL)
	{
		return (1);
	}
	struct dirent* entity;
	entity = readdir(dir);
	while (entity != NULL)
	{
		printf("%s\n", entity->d_name);
		entity = readdir(dir);
	}

	closedir(dir);
	return (0);
}