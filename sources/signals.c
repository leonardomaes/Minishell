/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 00:39:31 by rda-cunh          #+#    #+#             */
/*   Updated: 2024/12/07 18:17:50 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//process signal handlers

//used in interactive/shell mode to deal with SIGINT (Ctr+C)
void	ft_sigint_shell(int sig)
{
	g_signal = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0); //clears the current input line
	rl_on_new_line(); //move readline to a new line
	rl_redisplay(); //show new prompt
	(void) sig;
}

//handler in command execution to deal with SIGINT (Ctr+C) 
void	ft_sigint_command(int sig)
{
	g_signal = 130;
	write(1, "\n", 1);
	(void) sig; //do not have to deal with prompt
}

//handler to deal with SIGQUIT (Ctr+\)
void	ft_sigquit(int sig)
{
	g_signal = 131;
	write(1, "Quit (core dumped)\n", 19); //check with Leo about ft_printf | prints a standard message 
	(void) sig;
}

//child signal handlers | heredoc signal handlers


//manage different signals according with with operation mode
void	set_signal(int sg, t_msh *msh)
{
	if (sg == SHELL_MODE)
	{
		signal(SIGINT, ft_sigint_shell);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sg == COMMAND_MODE)
	{
		signal(SIGINT, ft_sigint_command);
		signal(SIGQUIT, ft_sigquit);
	}
	if (sg == EXIT) //handles EOF (Ctrl+D) in main and EOF in Heredoc or input file
	{
		write(1, "exit\n", 5);
		ft_free_all(msh);
		exit(EXIT_SUCCESS); //to decide later if we can include the exit a proper free and exit functions
	}
/* //to do 
	if (sg == HEREDOC)
	{

	}
	if (sg == HEREDOC_PAUSE)
	{

	}
*/
}
