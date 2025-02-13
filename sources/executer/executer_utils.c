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

int	exec_builtin(t_msh *msh, t_tokens *tokens)
{
	if (tokens->type == BLT_ECHO)
		return (execute_echo(msh, tokens->args));
	else if (tokens->type == BLT_CD)
		return (execute_cd(msh, tokens->args));
	else if (tokens->type == BLT_PWD)
		return (execute_pwd());
	else if (tokens->type == BLT_EXPORT)
		return (execute_export(msh, tokens->args));
	else if (tokens->type == BLT_UNSET)
		return (execute_unset(msh, tokens->args));
	else if (tokens->type == BLT_ENV)
		return (execute_env(tokens->args, msh->envp));
	else if (tokens->type == BLT_EXIT)
		return (execute_exit(msh, tokens->args));
	return (1);
}

void	handle_heredocs(t_msh *msh, t_tokens *token)
{
	int	fd;

	while (token && token->type != TKN_PIPE)
	{
		if (token->type == TKN_HEREDOC)
		{
			if (g_exit != 130)
			{
				fd = open(".heredoc_tmp", O_RDONLY);
				if (fd < 0)
					ft_perror(msh, "open heredoc_tmp", 1);
				if (dup2(fd, STDIN_FILENO) == -1)
					ft_perror(msh, "dup2", 1);
				close(fd);
				break ;
			}
			else
			{
				unlink(".heredoc_tmp");
				ft_free_all(msh);
				exit(130);
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

int	handle_redirs_one(t_msh *msh, t_tokens *token)
{
	if (open_files(msh, token) != 0)
	{
		g_exit = 1;
		return (-1);
	}
	if (msh->data->infile > 0)
		dup2(msh->data->infile, STDIN_FILENO);
	if (msh->data->outfile > 0)
		dup2(msh->data->outfile, STDOUT_FILENO);
	return (0);
}

int	handle_redirs_multi(t_msh *msh, t_tokens *token, int prev_pipe)
{
	if (open_files(msh, token) != 0)
	{
		g_exit = 1;
		ft_free_all(msh);
		exit(1);
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
	return (0);
}
