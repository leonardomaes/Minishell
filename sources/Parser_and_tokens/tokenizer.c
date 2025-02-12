/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:39 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_isdelimiter(char c)
{
	return (c == '"' || c == '$' || c == '\'' || c == '\0');
}

int	ft_isredirection(char c)
{
	return (c == '<' || c == '>');
}

void	split_tokens(t_msh *msh, t_tokens **token, t_tokens *prev, int i)
{
	t_tokens	*temp;

	if (i < msh->data->argc && msh->data->args[i][0] == '\0')
		i++;
	if (i < msh->data->argc)
	{
		if (!msh || !msh->data)
			return ;
		*token = NULL;
		*token = malloc(sizeof(t_tokens));
		if (!*token)
		{
			g_exit = 1;
			return ;
		}
		temp = *token;
		temp->name = msh->data->args[i];
		temp->args = NULL;
		if (prev == NULL || prev->type == TKN_PIPE)
			temp->type = get_type(msh, msh->data->args[i], i);
		else // Somente entra em get_type se for primeiro argumento
			temp->type = get_meta_type(msh, msh->data->args[i], 1);
		// Handle heredoc case
		if (temp->type == TKN_PIPE) //Se for pipe incrementa soma
			msh->data->pipes++;
		temp->count = msh->data->pipes; // Contagem do token no conjunto
		if (prev != NULL)				// Aponta para o token anterior
			temp->prev = prev;
		else
			temp->prev = NULL;
		if (temp->type == TKN_HEREDOC)	// Heredocs configs
		{
			while (i + 1 < msh->data->argc && msh->data->args[i + 1][0] == ' ')
				i++;
		}
		temp->next = NULL;
		i++;
		split_tokens(msh, &temp->next, temp, i);
	}
}
