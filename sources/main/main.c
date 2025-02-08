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

#include "../../minishell.h"

int	g_exit;

int main(int argc, char *argv[], char **envp)
{
	t_msh *msh;

	(void)argv;
	(void)argc;
	ft_init_shell(&msh, envp);				// Inicia struct principal
	while (1)
	{
		if (ft_readline(msh) == 0)
		{
			if (msh->data != NULL)
				execute(msh);
			ft_free_data(msh);
		}
		else if (!msh->data) //included this check to implement EOF signal (Ctr+D)
		{ // Entra aqui quando tem infile - solucionado, enterpretava como EOF
			set_signal(EXIT, msh);
			break ;
		}
	}
	ft_free_all(msh);
	return (0);
}
