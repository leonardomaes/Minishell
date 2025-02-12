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

//counts the number of args in a array
int	ft_countargs(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
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
		else
			temp->type = get_meta_type(msh, msh->data->args[i], 1);
		if (temp->type == TKN_PIPE)
			msh->data->pipes++;
		temp->count = msh->data->pipes;
		if (prev != NULL)
			temp->prev = prev;
		else
			temp->prev = NULL;
		if (temp->type == TKN_HEREDOC)
		{
			while (i + 1 < msh->data->argc && msh->data->args[i + 1][0] == ' ')
				i++;
		}
		temp->next = NULL;
		i++;
		split_tokens(msh, &temp->next, temp, i);
	}
}
