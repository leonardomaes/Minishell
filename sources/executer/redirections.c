/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:32:10 by lmaes             #+#    #+#             */
/*   Updated: 2025/01/11 12:46:15 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	open_append(t_msh *msh, t_tokens *temp)
{
	char	*filename;

	if (msh->data->outfile > 0)
		close(msh->data->outfile);
	filename = find_last_arg(temp);
	if (!filename)
		return (-1);
	msh->data->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0775);
	if (msh->data->outfile < 0)
	{
		perror("bash: ");
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);
}

int	open_outfile(t_msh *msh, t_tokens *temp)
{
	char	*filename;

	if (msh->data->outfile > 0)
		close(msh->data->outfile);
	filename = find_last_arg(temp);
	if (!filename)
		return (-1);
	msh->data->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (msh->data->outfile < 1)
	{
		perror("bash: ");
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);
}

int	open_infile(t_msh *msh, t_tokens *temp)
{
	char	*filename;

	filename = find_last_arg(temp);
	if (!filename)
		return (-1);
	if (msh->data->infile > 0)
		close(msh->data->infile);
	msh->data->infile = open(filename, O_RDONLY);
	if (msh->data->infile < 1)
	{
		perror("bash: ");
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);
}

void	init_files(t_msh *msh)
{
	msh->data->infile = -1;
	msh->data->outfile = -1;
	msh->data->stdin_backup = dup(STDIN_FILENO);
	msh->data->stdout_backup = dup(STDOUT_FILENO);
}

int	open_files(t_msh *msh, t_tokens *token)
{
	t_tokens	*temp;
	int			error;

	error = 0;
	temp = token;
	init_files(msh);
	while (temp->next != NULL && temp->type != TKN_PIPE)
	{
		if (temp->type == TKN_APPEND)
			error = open_append(msh, temp);
		else if (temp->type == TKN_OUTREDIR)
			error = open_outfile(msh, temp);
		else if (temp->type == TKN_INREDIR)
			error = open_infile(msh, temp);
		if (error != 0)
		{
			if (msh->data->infile > 0)
				close(msh->data->infile);
			if (msh->data->outfile > 0)
				close(msh->data->outfile);
			return (-1);
		}
		temp = temp->next;
	}
	return (0);
}
