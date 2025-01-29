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

char *find_last_arg(t_tokens *temp)
{
	t_tokens *tmp;
	char *file;

	tmp = temp;
	file = NULL;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
		if (tmp->type == ARGUMENT || tmp->type == DBL_QUOTES || tmp->type == SNG_QUOTES)
		{
			if (file != NULL)
			{
				free(file);
				file = NULL;
			}
			if (tmp->type == DBL_QUOTES)
				file = ft_chartrim(&tmp->name, '"');
			else if (tmp->type == SNG_QUOTES)
				file = ft_chartrim(&tmp->name, '\'');
			else
				file = ft_strdup(tmp->name);
		}
	}
	return (file);
}

int	open_append(t_msh *msh, t_tokens *temp)
{
	char *filename;

	if (msh->data->outfile > 0)
		close(msh->data->outfile);
	filename = find_last_arg(temp);
	if (!filename)
		return (-1);
	
	msh->data->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0775);
	if (msh->data->outfile < 0)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);

}

int open_outfile(t_msh *msh, t_tokens *temp)
{
	char *filename;

	if (msh->data->outfile > 0)
		close(msh->data->outfile);
	filename = find_last_arg(temp);
	if (!filename)
		return (-1);
	msh->data->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (msh->data->outfile < 1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);
}

int open_infile(t_msh *msh, t_tokens *temp)
{
	char *filename;

	if (msh->data->infile > 0)
		close(msh->data->infile);
	filename = find_last_arg(temp);
	if (!filename)
		return (-1);
	msh->data->infile = open(filename, O_RDONLY);
	if (msh->data->infile < 1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);
}

int open_files(t_msh *msh)
{
    t_tokens *temp = msh->data->tokens;
    int error = 0;

    msh->data->stdin_backup = dup(STDIN_FILENO);
    msh->data->stdout_backup = dup(STDOUT_FILENO);

    while (temp) // Percorre todos os tokens
    {
        if (temp->type == TKN_APPEND)
            error = open_append(msh, temp);
        else if (temp->type == TKN_OUTREDIR)
            error = open_outfile(msh, temp);
        else if (temp->type == TKN_INREDIR)
            error = open_infile(msh, temp);

        if (error != 0) // Se um erro ocorrer, fecha os arquivos abertos e retorna
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


/* 
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
			//printf("Append\n");
			if (msh->data->outfile > 0)
				close(msh->data->outfile);
			msh->data->outfile = open(temp->next->name, O_WRONLY | O_CREAT | O_APPEND, 0775);
			if (msh->data->outfile < 1)
			{
				ft_putstr_fd("bash: ", 2);
				ft_putstr_fd(temp->next->name, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				return (-1);
			}
		}
		else if (temp->type == TKN_OUTREDIR)
		{
			//printf("Output\n");
			if (msh->data->outfile > 0)
				close(msh->data->outfile);
			msh->data->outfile = open(temp->next->name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (msh->data->outfile < 1)
			{
				ft_putstr_fd("bash: ", 2);
				ft_putstr_fd(temp->next->name, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
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
				ft_putstr_fd("bash: ", 2);
				ft_putstr_fd(temp->next->name, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				return (-1);
			}
		}
		
		temp = temp->next;
	}
	return (0);
} */