/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 00:39:31 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/02/11 13:49:24 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
void	ft_sigint_child(int sig)
{
	write(1, "\n", 1);
	(void)sig;
}
