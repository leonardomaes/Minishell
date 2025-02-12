/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:14 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 02:49:29 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free_tokens(t_tokens *tokens)
{
	t_tokens	*temp;
	int			i;

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

void	ft_free_all(t_msh *msh)
{
	int	i;

	i = 0;
	while (msh->envp[i])
		free(msh->envp[i++]);
	free(msh->envp);
	ft_free_data(msh);
	free(msh);
}

void	ft_free_data(t_msh	*msh)
{
	int	i;

	i = 0;
	if (msh->data)
	{
		if (msh->data->cmd_paths)
		{
			i = 0;
			while (msh->data->cmd_paths[i])
				free(msh->data->cmd_paths[i++]);
			free(msh->data->cmd_paths);
		}
		if (msh->data->tokens)
			ft_free_tokens(msh->data->tokens);
		msh->data->tokens = NULL;
		i = 0;
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

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
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
