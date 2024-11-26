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
	free(msh->data->argv);
	free(msh->data);
}

void free_all(t_msh *msh)
{
	int i;

	i = 0;
	while (msh->envp[i])
		free(msh->envp[i++]);
	free(msh->envp);
	free(msh);
}

int main(int argc, char *argv[], char **envp)
{
	t_msh *msh;

	argv = NULL;
	argc = 0;
	init_shell(&msh, envp);				// Inicia struct principal
	//print_envp(msh->envp);			// Imprimir variavel ambiente
	msh->data = ft_readline();
	if (msh->data && msh->data->argv)
		printf("%s\n", msh->data->argv);
	free_data(msh);
	free_all(msh);
	return (0);
}