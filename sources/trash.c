/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trash.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:14:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/25 19:14:01 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_print_splitargs(char **args)
{
	int i;
	i = 0;
	while (args[i])
		printf("%s\n", args[i++]);
}

void	ft_print_params(t_msh *msh, t_data *data)
{
	ft_print_splitargs(data->args);
	printf("argc = %i\n", data->argc);
	printf("\n <-CMD PATHS->\n");
	ft_print_splitargs(msh->cmd_paths);
	printf("<--------->\n\n");
}
