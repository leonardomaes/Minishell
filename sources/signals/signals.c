/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 00:39:31 by rda-cunh          #+#    #+#             */
/*   Updated: 2024/12/09 19:14:48 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//process signal handlers

//used in interactive/shell mode to deal with SIGINT (Ctr+C)
void	ft_sigint_shell(int sig)
{
	g_exit = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void) sig;
}

//child signal handler for heredoc
void	child_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_exit = 130;
		write(1, "\n", 1);
		exit(g_exit);
	}
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
	if (sg == SHELL_MODE || sg == COMMAND_MODE || sg == CHILD_MODE)
		set_signal_mode(sg);
	if (sg == HEREDOC)
	{
		signal(SIGINT, child_signal_handler);
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
