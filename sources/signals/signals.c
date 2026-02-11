/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 00:39:31 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/02/11 13:49:24 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//helper function to store *msh and pass it to child signal handler
static t_msh	*get_msh(t_msh *msh)
{
	static t_msh	*stored_msh;

	if (msh != NULL)
		stored_msh = msh;
	return (stored_msh);
}

//child signal handler for heredoc
void	ft_sigint_heredoc(int sig)
{
	g_exit = 130;
	write(1, "\n", 1);
	ft_free_all(get_msh(NULL));
	exit(g_exit);
	(void)sig;
}

//handler function for set_signal | groups normal execution modes
static void	set_signal_mode(int sg)
{
	if (sg == SHELL_MODE)
	{
		signal(SIGINT, ft_sigint_shell);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sg == COMMAND_MODE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sg == CHILD_MODE)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}

//manage different signals according with with operation mode
void	set_signal(int sg, t_msh *msh)
{
	get_msh(msh);
	if (sg == SHELL_MODE || sg == COMMAND_MODE || sg == CHILD_MODE)
		set_signal_mode(sg);
	if (sg == HEREDOC)
	{
		signal(SIGINT, ft_sigint_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sg == HEREDOC_PAUSE)
		signal(SIGINT, SIG_IGN);
	if (sg == EXIT)
	{
		printf("exit\n");
		ft_free_all(msh);
		exit(EXIT_SUCCESS);
	}
}
