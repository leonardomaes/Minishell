/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:20:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/13 19:20:01 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_delimiter(t_msh *msh, char *data_args)
{
	if (get_type(msh, data_args, 1) == TKN_PIPE)
		return (TKN_PIPE);
	else if (get_type(msh, data_args, 1) == TKN_INREDIR)
		return (TKN_INREDIR);
	else if (get_type(msh, data_args, 1) == TKN_OUTREDIR)
		return (TKN_OUTREDIR);
	else if (get_type(msh, data_args, 1) == TKN_APPEND)
		return (TKN_APPEND);
	else if (get_meta_type(msh, data_args, 1) == TKN_HEREDOC)
		return (TKN_HEREDOC);
	return (0);
}

char	*return_arg(t_tokens *token)
{
	if (token->type == DBL_QUOTES)
		return (ft_chartrim(&token->name, '"'));
	else if (token->type == SNG_QUOTES)
		return (ft_chartrim(&token->name, '\''));
	else
		return (ft_strdup(token->name));
}

void	skip_delimiters(t_tokens **temp)
{
	*temp = (*temp)->next;
	if (*temp && (*temp)->type == TKN_SPACE)
		*temp = (*temp)->next;
	while (*temp && (*temp)->type != TKN_SPACE && (*temp)->type != TKN_PIPE)
		*temp = (*temp)->next;
}

char	*merge_args(t_tokens **temp)
{
	char	*merged;
	char	*next_arg;
	char	*str;

	merged = return_arg(*temp);
	*temp = (*temp)->next;
	while ((*temp) && (*temp)->type != TKN_PIPE && (*temp)->type != TKN_SPACE)
	{
		next_arg = return_arg(*temp);
		str = merged;
		merged = ft_strjoin(merged, next_arg);
		free(str);
		free(next_arg);
		*temp = (*temp)->next;
	}
	return (merged);
}

/*
void	getargs_cycle(t_msh *msh, t_tokens *temp, char ***args)
{
	int		i;

	i = 0;
	while (temp && temp->type != TKN_PIPE)
	{
		if (get_delimiter(msh, temp->name) != 0)
			skip_delimiters(&temp);
		else if (temp && temp->type != TKN_SPACE)
			(*args)[i++] = merge_args(&temp);
		temp = temp->next;
	}
	(*args)[i] = NULL;
}


void	getargs_cycle(t_msh *msh, t_tokens *temp, char ***args)
{
	int		i;
	char	*merged;
	char	*next_arg;
	char	*str;

	i = 0;
	while (temp && temp->type != TKN_PIPE)
	{
		if (get_delimiter(msh, temp->name) != 0)
			skip_delimiters(&temp);
		else if (temp && temp->type != TKN_SPACE)
		{
			merged = return_arg(temp);
			temp = temp->next;
			while (temp && temp->type != TKN_PIPE && temp->type != TKN_SPACE)
			{
				next_arg = return_arg(temp);
				str = merged;
				merged = ft_strjoin(merged, next_arg);
				free(str);
				free(next_arg);
				temp = temp->next;
			}
			(*args)[i++] = merged;
		}
		else
			temp = temp->next;
	}
	(*args)[i] = NULL;
}


void	getargs_cycle(t_msh *msh, t_tokens *temp, char ***args)
{
	int		i;
	char	*merged;
	char	*next_arg;
	char	*str;

	i = 0;
	while (temp && temp->type != TKN_PIPE)
	{
		if (get_delimiter(msh, temp->name) != 0)
		{
			temp = temp->next;
			if (temp && temp->type == TKN_SPACE)
				temp = temp->next;
			while (temp && temp->type != TKN_SPACE && temp->type != TKN_PIPE)
				temp = temp->next;
			continue;
		}
		else if (temp && temp->type != TKN_SPACE)
		{
			merged = return_arg(temp);
			temp = temp->next;
			while (temp && temp->type != TKN_PIPE && temp->type != TKN_SPACE)
			{
				next_arg = return_arg(temp);
				str = merged;
				merged = ft_strjoin(merged, next_arg);
				free(str);
				free(next_arg);
				temp = temp->next;
			}
			(*args)[i++] = merged;
		}
		else
			temp = temp->next;
	}
	(*args)[i] = NULL;
}

*/