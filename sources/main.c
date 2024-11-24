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



void free_all(t_msh *msh)
{
	free(msh->envp);
	free(msh);
}

int main(int argc, char *argv[], char **envp)
{
	t_msh *msh;

	argv = NULL;
	argc = 0;
	msh = init_shell(envp);				// Inicia struct principal
	//print_envp(msh->envp);			// Imprimir variavel ambiente
	while (1)							// Ciclo do terminal
	{
		msh->data = ft_readline();		// Recebe os dados na struct e faz o parser
		
	}
	//free_all(&msh);
	return (0);
}