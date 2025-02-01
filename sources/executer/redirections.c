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
    t_tokens *tmp = temp;
    char *file = NULL;
    char *joined_str = NULL;
    char *temp_str = NULL;

    while (tmp && tmp->next && tmp->next->type != TKN_PIPE && tmp->next->type != TKN_OUTREDIR && tmp->next->type != TKN_INREDIR && tmp->next->type != TKN_APPEND)
    {
        tmp = tmp->next;
        if (tmp->type == ARGUMENT || tmp->type == DBL_QUOTES || tmp->type == SNG_QUOTES)
        {
            if (tmp->type == DBL_QUOTES)
                temp_str = ft_chartrim(&tmp->name, '"');
            else if (tmp->type == SNG_QUOTES)
                temp_str = ft_chartrim(&tmp->name, '\'');
            else
                temp_str = ft_strdup(tmp->name);

            if (!file) // Primeiro argumento
                file = temp_str;
            else
            {
                joined_str = ft_strjoin(file, temp_str); // Concatena strings
                free(file);
                free(temp_str);
                file = joined_str;
            }

            if (!tmp->next || tmp->next->type == TKN_SPACE) // Se próximo for espaço, retorna
                return (file);
        }
    }
    return (file);
}


 // Adicionar condição para fazer strjoin na sequencia de strings até achar o espaço

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
		perror("bash: ");
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
		perror("bash: ");
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);
}

int open_infile(t_msh *msh, t_tokens *temp)
{
	char *filename;

	filename = find_last_arg(temp);
	if (!filename)
		return (-1);
	if (msh->data->infile > 0)
		close(msh->data->infile);
	//printf("-%s\n", filename);
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

int	close_files(t_msh *msh)
{
	if (msh->data->infile > 0)
	{
		close(msh->data->infile);
		msh->data->infile = -1;
		dup2(msh->data->stdin_backup, STDIN_FILENO);
		close(msh->data->stdin_backup);
	}
	if (msh->data->outfile > 0)
	{
		close(msh->data->outfile);
		msh->data->outfile = -1;
		dup2(msh->data->stdout_backup, STDOUT_FILENO);
		close(msh->data->stdout_backup);
	}
	return (0);
}

int open_files(t_msh *msh, t_tokens *token)
{
	t_tokens *temp;
	int error = 0;

	temp = token;
	msh->data->stdin_backup = dup(STDIN_FILENO);
	msh->data->stdout_backup = dup(STDOUT_FILENO);
	while (temp != NULL && temp->type != TKN_PIPE) // Percorre todos os tokens
	{
		//printf("-%s\n", temp->name);
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