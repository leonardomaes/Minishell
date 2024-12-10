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

	while (tokens != NULL)
	{
		temp = tokens;
		tokens = tokens->next;
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
	ft_free_tokens(msh->data->tokens);
	msh->data->tokens = NULL;
	while (msh->data->args[i])
		free(msh->data->args[i++]);
	free(msh->data->args);
	free(msh->data);
}

