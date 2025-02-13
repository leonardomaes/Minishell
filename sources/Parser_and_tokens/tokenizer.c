/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/13 02:09:05 by rda-cunh         ###   ########.fr       */
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

int	ft_countargs(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

static void	init_token(t_tokens *temp, t_tokens *prev, t_msh *msh, int i)
{
	temp->name = msh->data->args[i];
	temp->args = NULL;
	if (prev == NULL || prev->type == TKN_PIPE)
		temp->type = get_type(msh, msh->data->args[i], i);
	else
		temp->type = get_meta_type(msh, msh->data->args[i], 1);
	if (temp->type == TKN_PIPE)
		msh->data->pipes++;
	temp->count = msh->data->pipes;
	temp->prev = prev;
	temp->next = NULL;
}

void	split_tokens(t_msh *msh, t_tokens **token, t_tokens *prev, int i)
{
	t_tokens	*temp;

	if (i < msh->data->argc && msh->data->args[i][0] == '\0')
		i++;
	if (i >= msh->data->argc || !msh || !msh->data)
		return ;
	*token = malloc(sizeof(t_tokens));
	if (!*token)
	{
		g_exit = 1;
		return ;
	}
	temp = *token;
	init_token(temp, prev, msh, i);
	if (temp->type == TKN_HEREDOC)
		while (i + 1 < msh->data->argc && msh->data->args[i + 1][0] == ' ')
			i++;
	split_tokens(msh, &temp->next, temp, i + 1);
}
