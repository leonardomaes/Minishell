/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:20:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/13 19:20:01 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	alloc_getargs(t_msh *msh, t_tokens *token)
{
	int			i;
	t_tokens	*temp;

	i = 0;
	temp = token;
	while (temp && temp->type != TKN_PIPE)
	{
		if (get_delimiter(msh, temp->name) != 0)
		{
			if (temp->next)
				temp = temp->next;
			if (token && (token)->type == TKN_SPACE)
				token = token->next;
			while (temp && temp->type != TKN_SPACE)
				temp = temp->next;
			while (temp && temp->type == TKN_SPACE)
				temp = temp->next;
			continue ;
		}
		if (temp && temp->type != TKN_SPACE)
			i++;
		if (temp)
			temp = temp->next;
	}
	return (i);
}

void	getargs_cycle(t_msh *msh, t_tokens **temp, int size)
{
	t_tokens	*token;
	t_tokens	*tmp;
	int			i;

	if (!temp || !(*temp))
		return ;
	token = *temp;
	ft_skip_delimiters(msh, &token);
	tmp = token;
	if (!tmp)
		return ;
	tmp->args = malloc(sizeof(char *) * (size + 1));
	if (!tmp->args)
		ft_perror(msh, "malloc:", 1);
	i = 0;
	while (token && token->type != TKN_PIPE)
	{
		if (get_delimiter(msh, token->name) != 0)
			skip_redirs(&token);
		else if (token && token->type != TKN_SPACE)
			tmp->args[i++] = merge_args(&token);
		else
			token = token->next;
	}
	tmp->args[i] = NULL;
}

void	get_args(t_msh *msh, t_tokens	**tokens)
{
	t_tokens	*temp;
	int			i;

	i = 0;
	temp = *tokens;
	i = alloc_getargs(msh, temp);
	if (i == 0)
		i++;
	getargs_cycle(msh, tokens, i);
}

void	get_heredoc_args(t_msh *msh, t_tokens **temp)
{
	t_tokens	*i;

	i = (*temp)->next;
	(*temp)->args = malloc(sizeof(char *) * 2);
	if (!(*temp)->args)
		ft_perror(msh, "malloc:", 1);
	(*temp)->args[0] = NULL;
	if (i && i->type == TKN_SPACE)
		i = i->next;
	if (i)
		(*temp)->args[0] = merge_args(&i);
	(*temp)->args[1] = NULL;
}

void	ft_get_args(t_msh *msh)
{
	t_tokens	*temp;

	temp = msh->data->tokens;
	while (temp)
	{
		if (temp->type == TKN_HEREDOC)
			get_heredoc_args(msh, &temp);
		else
			temp->args = NULL;
		if (temp)
			temp = temp->next;
	}
	temp = msh->data->tokens;
	while (temp)
	{
		if (!temp->prev || temp->prev->type == TKN_PIPE)
			get_args(msh, &temp);
		if (temp)
			temp = temp->next;
	}
}
