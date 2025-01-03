/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/09 18:52:04 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_exit;

int main(int argc, char *argv[], char **envp)
{
	t_msh *msh;

	(void)argv;
	(void)argc;
	ft_init_shell(&msh, envp);				// Inicia struct principal
	while (1)
	{
		ft_readline(msh);
		// Entra aqui e faz exit caso line == '\0'
		if (!msh->data) //included this check to implement EOF signal (Ctr+D)
		{
			set_signal(EXIT, msh);
			break ;
		}
		//msh->envp[27] = ft_strdup("teste");
		if (msh->data->args[0] && !ft_strncmp(msh->data->args[0], "exit", 4))	// test
			break;
		/* if (!ft_strncmp(msh->data->args[0], "env", 3))							// test
			exec_env(msh->envp); */
		if (msh->data != NULL)
			execute(msh);
		ft_free_data(msh);
	}
	ft_free_all(msh);
	return (0);
}
