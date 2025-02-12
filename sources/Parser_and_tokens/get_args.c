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

int get_delimiter(t_msh *msh, char *data_args)
{
	if (get_type(msh, data_args, 1) == TKN_PIPE)
		return (TKN_PIPE);
	else if(get_type(msh, data_args, 1) == TKN_INREDIR)
		return (TKN_INREDIR);
	else if(get_type(msh, data_args, 1) == TKN_OUTREDIR)
		return (TKN_OUTREDIR);
	else if(get_type(msh, data_args, 1) == TKN_APPEND)
		return (TKN_APPEND);
	else if (get_meta_type(msh, data_args, 1) == TKN_HEREDOC)
		return (TKN_HEREDOC);
	return (0);
}

char	**getargs(t_msh *msh, t_tokens *token)
{
	int		i;
	char	**args;
	char	*merged;
	char	*next_arg;
	char	*str;
	t_tokens *temp;

	temp = token;
	i = 0;
	while (temp && temp->type != TKN_PIPE)
	{
		if (get_delimiter(msh, temp->name) != 0) // Avança redirects
		{
			temp = temp->next;
			if (temp->type == TKN_SPACE)
				temp = temp->next;
			if (temp->type != TKN_PIPE)
				temp = temp->next;
			continue ;
		}
		if (temp && temp->type != TKN_SPACE)
			i++;
		temp = temp->next;
	}
	if (i == 0)
		i++;
	args = malloc(sizeof(char *) * (i + 1));
	if (!args)
	{
		perror("malloc:");
		ft_free_all(msh);
		exit(1);
	}
	i = 0;
	temp = token;
	while (temp && temp->type != TKN_PIPE)
	{
		if (get_delimiter(msh, temp->name) != 0) // Caso seja um redirect ou pipe apenas os avança
		{
			temp = temp->next;
			if (temp && temp->type == TKN_SPACE)
				temp = temp->next;
			while (temp && temp->type != TKN_SPACE && temp->type != TKN_PIPE)
				temp = temp->next;
			continue ;
		}
		else if (temp && temp->type != TKN_SPACE)
		{
			if (temp->type == DBL_QUOTES)
				merged = ft_chartrim(&temp->name, '"');
			else if (temp->type == SNG_QUOTES)
				merged = ft_chartrim(&temp->name, '\'');
			else
				merged = ft_strdup(temp->name);
			temp = temp->next;
			while (temp && temp->type != TKN_PIPE && temp->type != TKN_SPACE)
			{
				if (temp->type == DBL_QUOTES)
					next_arg = ft_chartrim(&temp->name, '"');
				else if (temp->type == SNG_QUOTES)
					next_arg = ft_chartrim(&temp->name, '\'');
				else
					next_arg = ft_strdup(temp->name);
				str = merged;
				merged = ft_strjoin(merged, next_arg);
				free(str);
				free(next_arg);
				temp = temp->next;
			}
			args[i++] = merged;
		}
		else
			temp = temp->next;
	}
	args[i] = NULL;
	return (args);
}

void	ft_get_args(t_msh *msh)
{
	t_tokens	*temp;
	t_tokens	*i;

	temp = msh->data->tokens;
	while (temp)
	{
		if (temp->type == TKN_HEREDOC)
		{
			i = temp->next;
			temp->args = malloc(sizeof(char *) * 2);
			if (!temp->args)
			{// Talvez arrumar esta saida
				free(temp);
				temp = NULL;
				return ;
			}
			temp->args[0] = NULL;
			if (i && i->type == TKN_SPACE)
				i = i->next;
			if (i && i->type == DBL_QUOTES)
				temp->args[0] = ft_chartrim(&i->name, '"');
			else if (i && i->type == SNG_QUOTES)
				temp->args[0] = ft_chartrim(&i->name, '\'');
			else if (i)
				temp->args[0] = ft_strdup(i->name);
			temp->args[1] = NULL;
		}
		else if (!temp->prev || temp->prev->type == TKN_PIPE)
			temp->args = getargs(msh, temp);
		else
			temp->args = NULL;
		temp = temp->next;
	}
}
