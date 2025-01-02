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
	printf("\n <-CMD PATHS->\n");
	while (args[i])
		printf("%s<\n", args[i++]);
}

void	ft_print_params(t_msh *msh)
{
	ft_print_splitargs(msh->data->args);
	printf("argc = %i\n", msh->data->argc);
	//ft_print_splitargs(msh->cmd_paths);
	printf("<--------->\n\n");
}

void	ft_print_tokens(t_tokens *tokens)
{
	t_tokens *temp;

	temp = tokens;
	while (temp != NULL)
	{
		printf("\nArg n°%i", temp->count);
		printf("\nName - %s<", temp->name);
		printf("\nType - %i<\n", temp->type);
		temp = temp->next;
	}
}
