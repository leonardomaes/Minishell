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

int open_files(t_msh *msh)
{
	t_tokens *temp;

	temp = msh->data->tokens;
	msh->data->stdin_backup = dup(STDIN_FILENO);
	msh->data->stdout_backup = dup(STDOUT_FILENO);
	while (temp) // Loop por todos os tokens
	{
		if (temp->type == TKN_APPEND)
		{
			printf("Append\n");
			if (msh->data->outfile > 0)
				close(msh->data->outfile);
			msh->data->outfile = open(temp->next->name, O_WRONLY | O_CREAT | O_APPEND, 0775);
			if (msh->data->outfile < 1)
			{
				printf("bash: %s: No such file or directory\n", temp->next->name);
				return (-1);
			}
		}
		else if (temp->type == TKN_OUTREDIR)
		{
			printf("Output\n");
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
