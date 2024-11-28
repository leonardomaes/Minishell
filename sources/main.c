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

void free_data(t_msh *msh)
{
	free(msh->data->cmds);
}

void free_all(t_msh *msh)
{
	int i;

	i = 0;
	while (msh->envp[i])
		free(msh->envp[i++]);
	free(msh->envp);
	free(msh->data);
	free(msh);
}

int main(int argc, char *argv[], char **envp)
{
	t_msh *msh;

	(void)argv;
	(void)argc;
	init_shell(&msh, envp);				// Inicia struct principal
	//print_envp(msh->envp);			// Imprimir variavel ambiente
	while (1)
	{
		msh->data = ft_readline();
		
	}

	//free_data(msh);
	free_all(msh);
	return (0);
}