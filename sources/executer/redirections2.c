/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:16:20 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 20:16:21 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	close_files(t_msh *msh)
{
	if (msh->data->infile > 0)
	{
		close(msh->data->infile);
		msh->data->infile = -1;
		dup2(msh->data->stdin_backup, STDIN_FILENO);
		close(msh->data->stdin_backup);
	}
	if (msh->data->outfile > 0)
	{
		close(msh->data->outfile);
		msh->data->outfile = -1;
		dup2(msh->data->stdout_backup, STDOUT_FILENO);
		close(msh->data->stdout_backup);
	}
	return (0);
}

char	*get_token_value(t_tokens *tmp)
{
	if (tmp->type == DBL_QUOTES)
		return (ft_chartrim(&tmp->name, '"'));
	else if (tmp->type == SNG_QUOTES)
		return (ft_chartrim(&tmp->name, '\''));
	return (ft_strdup(tmp->name));
}

char	*get_joined_value(char *joined_str, char *file, char *temp_str)
{
	joined_str = ft_strjoin(file, temp_str);
	free(file);
	free(temp_str);
	return (joined_str);
}

int	ft_isredir(int	type)
{
	if (type == TKN_PIPE || type == TKN_OUTREDIR || type == TKN_INREDIR || type == TKN_APPEND)
		return (1);
	return (0);
}

char	*find_last_arg(t_tokens *temp)
{
	t_tokens	*tmp;
	char		*file;
	char		*joined_str;
	char		*temp_str;

	tmp = temp;
	file = NULL;
	joined_str = NULL;
	temp_str = NULL;
	while (tmp && tmp->next && ft_isredir(tmp->next->type) == 0)
	{
		tmp = tmp->next;
		if (tmp->type == ARGUMENT || tmp->type == DBL_QUOTES || tmp->type == SNG_QUOTES)
		{
			temp_str = get_token_value(tmp);
			if (!file)
				file = temp_str;
			else
				file = get_joined_value(joined_str, file, temp_str);
			if (!tmp->next || tmp->next->type == TKN_SPACE)
				return (file);
		}
	}
	return (file);
}
