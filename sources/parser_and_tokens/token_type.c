/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:39 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_builtin_type(char *name)
{
	if (name && ft_strcmp(name, "cd") == 0)
		return (BLT_CD);
	else if (name && ft_strcmp(name, "echo") == 0)
		return (BLT_ECHO);
	else if (name && ft_strcmp(name, "pwd") == 0)
		return (BLT_PWD);
	else if (name && ft_strcmp(name, "export") == 0)
		return (BLT_EXPORT);
	else if (name && ft_strcmp(name, "unset") == 0)
		return (BLT_UNSET);
	else if (name && ft_strcmp(name, "env") == 0)
		return (BLT_ENV);
	else if (name && ft_strcmp(name, "exit") == 0)
		return (BLT_EXIT);
	else
		return (0);
}

int	get_meta_type(t_msh *msh, char *name, int i)
{
	if (name && name[0] == '"')
		return (DBL_QUOTES);
	else if (name && name[0] == '\'')
		return (SNG_QUOTES);
	else if (name && name[0] == '|' && name[1] == '\0')
		return (TKN_PIPE);
	else if (name && name[0] == '$')
		return (VAR_ENVIRON);
	else if (name && name[0] == '<' && name[1] == '<' && name[2] == '\0')
		return (TKN_HEREDOC);
	else if (name && ft_strcmp(name, ">>") == 0)
		return (TKN_APPEND);
	else if (name && name[0] == '>')
		return (TKN_OUTREDIR);
	else if (name && name[0] == '<')
		return (TKN_INREDIR);
	else if (name && ((ft_isspace(name[0]) && !name[1]) || name[0] == '\0'))
		return (TKN_SPACE);
	else if (i == 0 || msh->data->args[i - 1][0] == '|')
		return (TKN_BCMD);
	else
		return (ARGUMENT);
}

int	get_type(t_msh *msh, char *name, int i)
{
	if (get_builtin_type(name) != 0)
		return (get_builtin_type(name));
	else if (get_meta_type(msh, name, i) != 0)
		return (get_meta_type(msh, name, i));
	else
		return (1);
}
