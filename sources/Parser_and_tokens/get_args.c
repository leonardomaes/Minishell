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

void	getargs_cycle(t_msh *msh, t_tokens *temp, char ***args)
{
	int		i;

	i = 0;
	while (temp && temp->type != TKN_PIPE)
	{
		if (get_delimiter(msh, temp->name) != 0)
			skip_delimiters(&temp);
		else if (temp && temp->type != TKN_SPACE)
		{
			(*args)[i++] = merge_args(&temp);
		}
		else
			temp = temp->next;
	}
	(*args)[i] = NULL;
}

char	**getargs(t_msh *msh, t_tokens *token)
{
	int			i;
	char		**args;

	args = NULL;
	i = alloc_getargs(msh, token);
	args = malloc(sizeof(char *) * (i + 1));
	if (!args)
		ft_perror(msh, "malloc:", 1);
	getargs_cycle(msh, token, &args);
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
			if (temp->prev == NULL)
			{
				while (temp && temp->type != TKN_SPACE)
					temp = temp->next;
				if (temp && temp->type == TKN_SPACE)
					temp = temp->next;
				
			}
			else
			{
				i = temp->next;
				temp->args = malloc(sizeof(char *) * 2);
				if (!temp->args)
					ft_perror(msh, "malloc:", 1);
				temp->args[0] = NULL;
				if (i && i->type == TKN_SPACE)
					i = i->next;
				temp->args[0] = return_arg(i);
				temp->args[1] = NULL;
			}
			
		}
		else if (!temp->prev || temp->prev->type == TKN_PIPE)
			temp->args = getargs(msh, temp);
		else
			temp->args = NULL;
		temp = temp->next;
	}
}
