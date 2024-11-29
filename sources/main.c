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

void ft_free_all(t_msh *msh)
{
	int i;

	i = 0;
	while (msh->envp[i])
		free(msh->envp[i++]);
	free(msh->envp);
	i = 0;
	while (msh->cmd_paths[i])
		free(msh->cmd_paths[i++]);
	free(msh->cmd_paths);
	i = 0;
	while (msh->data->args[i])
		free(msh->data->args[i++]);
	free(msh->data->args);
	free(msh->data);
	free(msh);
}

void ft_free_data(t_msh	*msh)
{
	int i;

	i = 0;
	while (msh->data->args[i])
		free(msh->data->args[i++]);
	free(msh->data->args);
	free(msh->data);
}

int main(int argc, char *argv[], char **envp)
{
	t_msh *msh;

	(void)argv;
	(void)argc;
	ft_init_shell(&msh, envp);				// Inicia struct principal
	while (1)
	{
		msh->data = ft_readline(msh);
		if (!ft_strncmp(msh->data->args[0], "exit", 4))
			break;
		ft_free_data(msh);
	}
	ft_free_all(msh);
	return (0);
}