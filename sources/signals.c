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

#include "../minishell.h"

//process signal handlers

//used in interactive/shell mode to deal with SIGINT (Ctr+C)
void	ft_sigint_shell(int sig)
{
	g_exit = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0); //clears the current input line
	rl_on_new_line(); //move readline to a new line
	rl_redisplay(); //show new prompt
	(void) sig;
}

//handler in command execution to deal with SIGINT (Ctr+C) 
void	ft_sigint_command(int sig)
{
	g_exit = 130;
	write(1, "\n", 1);
	(void) sig; //do not have to deal with prompt
}

//handler to deal with SIGQUIT (Ctr+\)
void	ft_sigquit(int sig)
{
	g_exit = 131;
	printf("Quit (core dumped)\n");
	(void) sig;
}

//child signal handlers | heredoc signal handlers
void	child_signal_handler(int sig)
{
	if (sig == HEREDOC)
	{
		g_exit = 130;
		write(1, "\n", 1);
		exit(g_exit);
	}
}

void	child_signal_handler2(int sig)
{
	if (sig == HEREDOC_PAUSE)
	{
		g_exit = 130;
		signal(SIGINT, SIG_IGN);
	}
}

//manage different signals according with with operation mode
void	set_signal(int sg, t_msh *msh)
{
	if (sg == SHELL_MODE)
	{
		signal(SIGINT, ft_sigint_shell);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sg == COMMAND_MODE) //must call set_signal(COMMAND_MODE, msh) right before executing commands (before fork) and got back to SHELL MODE after execution - TO DO!
	{
		signal(SIGINT, ft_sigint_command);
		signal(SIGQUIT, ft_sigquit);
	}
	if (sg == EXIT) //handles EOF (Ctrl+D) in main and EOF in Heredoc or input file | palaced after the readline function
	{
		printf("exit\n");
		ft_free_all(msh);
		exit(EXIT_SUCCESS); //to decide later if we can include the exit a proper free and exit functions
	}
	if (sg == HEREDOC) //call set_signal(HEREDOC, msh) before starting HEREDOC and return do SHELL_MODE after - TO DO!
		signal(SIGINT, child_signal_handler);
	if (sg == HEREDOC_PAUSE) //call set_signal(HEREDOC_PAUSE, msh) when pausing HEREDOC - TO DO!
		signal(SIGINT, child_signal_handler2);
}
