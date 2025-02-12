/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:20:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/13 19:20:01 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int exec_builtin(t_msh *msh, t_tokens *tokens)
{
	if (tokens->type == BLT_ECHO)
		return (execute_echo(msh, tokens->args));
	else if(tokens->type == BLT_CD)
		return (execute_cd(msh, tokens->args));
	else if(tokens->type == BLT_PWD)
		return (execute_pwd());
	else if(tokens->type == BLT_EXPORT)
		return (execute_export(msh, tokens->args));
	else if(tokens->type == BLT_UNSET)
		return (execute_unset(msh, tokens->args));
	else if(tokens->type == BLT_ENV)
		return (execute_env(tokens->args, msh->envp));
	else if(tokens->type == BLT_EXIT)
		return (execute_exit(msh, tokens->args));
	return (1);
}
 //added this to handle heredoc (must evaluate how to melt with redirections code from Leonardo)
void	handle_heredocs(t_msh *msh, t_tokens *token)
{
	int	fd;
	
	while (token && token->type != TKN_PIPE)
	{
		if (token->type == TKN_HEREDOC)
		{
			if (g_exit != 130)
			{
				fd = open(".heredoc_tmp", O_RDONLY); //opens heredoc file
				if (fd < 0)
					ft_perror(msh, "open heredoc_tmp", 1);
				if (dup2(fd, STDIN_FILENO) == -1) //redirects stdin to read from the heredoc file
					ft_perror(msh, "dup2", 1);
				close(fd);
				break ; //only use the last heredoc (multi heredocs)
			}
			else
			{
				unlink(".heredoc_tmp");
				ft_free_all(msh);
				exit (130);
			}
		}
		token = token->next;
	}
}

void	setup_heredocs(t_tokens *tokens, t_msh *msh)
{
	t_tokens	*current;

	current = tokens;
	while (current && current->type != TKN_PIPE)
	{
		if (current->type == TKN_HEREDOC && g_exit != 130)
			handle_heredoc(current, msh);
		current = current->next;
	}
}

void	handle_redirs(t_msh *msh, t_tokens *token, int prev_pipe)
{
	if (open_files(msh, token) != 0)
	{
		g_exit = 1;
		ft_free_all(msh);
		exit (1);
	}
	if (prev_pipe != -1) 
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (msh->data->infile > 0)
	{
		dup2(msh->data->infile, STDIN_FILENO);
		close(msh->data->infile);
	}
	if (msh->data->outfile > 0)
	{
		dup2(msh->data->outfile, STDOUT_FILENO);
		close(msh->data->outfile);
	}
}
