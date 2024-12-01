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

void	split_tokens(t_msh *msh, t_tokens **token, int i)
{
	t_tokens	*temp;
	
	if (i < msh->data->argc)
	{
		*token = NULL;
		*token = malloc(sizeof(t_tokens));
		temp = *token;
		temp->count = i;
		temp->name = msh->data->args[i];
		temp->type = get_type(msh->data->args[i]);
		/* if (i < msh->data->argc - 1)
			temp->next->prev = temp; */
		i++;
		temp->next = NULL;
		split_tokens(msh, &temp->next, i);
	}
}
