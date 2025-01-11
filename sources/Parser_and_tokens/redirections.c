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

void    handle_redirs(t_msh *msh, int i, t_tokens *temp)
{
	if (temp->type == TKN_OUTREDIR && msh->data->args[i+1])
	{
		msh->data->outfile = open(msh->data->args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (temp->type == TKN_INREDIR && msh->data->args[i+1])
	{
		msh->data->infile = open(msh->data->args[i+1], O_RDONLY);
	}
	else if (temp->type == TKN_APPEND && msh->data->args[i+1])
	{
		msh->data->outfile = open(msh->data->args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
	}
}

int open_files(t_msh *msh)
{
	t_tokens *temp;

	temp = msh->data->tokens;
	while (temp) // Loop por todos os tokens
	{
		if (temp->type == TKN_APPEND)
		{
			if (msh->data->outfile > 0)
				close(msh->data->outfile);
			msh->data->outfile = open(temp->next->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (msh->data->outfile < 1)
			{
				printf("bash: %s: No such file or directory\n", temp->next->name);
				return (-1);
			}
		}
		else if (temp->type == TKN_OUTREDIR)
		{
			if (msh->data->outfile > 0)
				close(msh->data->outfile);
			msh->data->outfile = open(temp->next->name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (msh->data->outfile < 1)
			{
				printf("bash: %s: No such file or directory\n", temp->next->name);
				return (-1);
			}
		}
		else if (temp->type == TKN_INREDIR)
		{
			if (msh->data->infile > 0)
				close(msh->data->infile);
			msh->data->infile = open(temp->next->name, O_RDONLY, 0777);
			if (msh->data->infile < 1)
			{
				printf("bash: %s: No such file or directory\n", temp->next->name);
				return (-1);
			}
		}
		
		temp = temp->next;
	}
	return (0);
}
