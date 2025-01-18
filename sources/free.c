/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:14 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/07 19:16:04 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_free_tokens(t_tokens *tokens)
{
	t_tokens *temp;
	int i;

	while (tokens != NULL)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->args != NULL)
		{
			i = 0;
			while (temp->args[i])
				free(temp->args[i++]);
			free(temp->args);
		}
		
		free(temp);
	}
}

void ft_free_all(t_msh *msh)
{
	int i;

	i = 0;
	while (msh->envp[i])
		free(msh->envp[i++]);
	free(msh->envp);
	i = 0;
	while (msh->cmd_paths[i])
		free(msh->cmd_paths[i++]);
	free(msh->cmd_paths);
	if (msh->data) //RM: need to review this function to avoid SEGFAULT, because i need to free msh->data dealing with EOF in readline
	{
		if (msh->data->tokens)
			ft_free_tokens(msh->data->tokens);
        if (msh->data->args)
		{
			i = 0;
			while (msh->data->args[i])
				free(msh->data->args[i++]);
			free(msh->data->args);
		}
		free(msh->data);
	}
	free(msh);
}

void ft_free_data(t_msh	*msh)
{
	int i;

	i = 0;
	if (msh->data)
	{
		if (msh->data->tokens)
			ft_free_tokens(msh->data->tokens);
		msh->data->tokens = NULL;
		while (msh->data->args[i])
			free(msh->data->args[i++]);
		free(msh->data->args);
		if (msh->data->infile > 0)
			close(msh->data->infile);
		else if (msh->data->outfile > 0)
			close(msh->data->outfile);
		free(msh->data);
	}
}

void	free_array(char **str, unsigned int n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

//standard funtion to free a pointer of any type and set it as NULL (avoids double free)
void	free_ptr(void **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}
