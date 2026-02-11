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

#include "../../includes/minishell.h"

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

void	ft_skip_delimiters(t_msh *msh, t_tokens **temp)
{
	while (*temp && get_delimiter(msh, (*temp)->name) != 0)
		skip_redirs(temp);
	while (*temp && (*temp)->type == TKN_SPACE)
		*temp = (*temp)->next;
}
