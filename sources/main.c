/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/07 18:37:48 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal;

int main(int argc, char *argv[], char **envp)
{
	t_msh *msh;

	(void)argv;
	(void)argc;
	ft_init_shell(&msh, envp);				// Inicia struct principal
	while (1)
	{
		ft_readline(msh);
		if (!msh->data) //included this check to implement EOF signal (Ctr+D)
		{
			set_signal(EXIT, msh);
			break; 
		}
		if (msh->data->args[0] && !ft_strncmp(msh->data->args[0], "exit", 4))
			break;
		ft_free_data(msh);
	}
	ft_free_all(msh);
	return (0);
}