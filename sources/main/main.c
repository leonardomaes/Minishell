/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:00 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 02:55:39 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_exit;

void	ft_init_shell(t_msh **msh, char **envp)
{
	g_exit = 0;
	set_signal(SHELL_MODE, NULL);
	(*msh) = malloc(sizeof(t_msh));
	if (!*msh)
		return ;
	(*msh)->envp = ft_get_env(envp);
}

int	main(int argc, char *argv[], char **envp)
{
	t_msh	*msh;

	(void)argv;
	(void)argc;
	ft_init_shell(&msh, envp);
	while (1)
	{
		if (ft_readline(msh) == 0)
		{
			if (msh->data != NULL)
				execute(msh);
			ft_free_data(msh);
		}
		else if (!msh->data)
		{
			set_signal(EXIT, msh);
			break ;
		}
	}
	ft_free_all(msh);
	return (0);
}
