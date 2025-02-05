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

#include "../../minishell.h"

int	get_builtin_type(char *name)
{
	if (ft_strcmp(name, "cd") == 0)
		return (BLT_CD);
	else if (ft_strcmp(name, "echo") == 0)
		return (BLT_ECHO);
	else if (ft_strcmp(name, "pwd") == 0)
		return (BLT_PWD);
	else if (ft_strcmp(name, "export") == 0)
		return (BLT_EXPORT);
	else if (ft_strcmp(name, "unset") == 0)
		return (BLT_UNSET);
	else if (ft_strcmp(name, "env") == 0)
		return (BLT_ENV);
	else if (ft_strcmp(name, "exit") == 0)
		return (BLT_EXIT);
	else
		return (0);
}

int get_meta_type(t_msh *msh, char *name, int i)
{
	if (name[0] == '"')
		return (DBL_QUOTES);
	else if (name[0] == '\'')
		return (SNG_QUOTES);
	else if (name[0] == '|')
		return (TKN_PIPE);
	else if (name[0] == '$')
		return (VAR_ENVIRON);
	else if (name[0] == '<' && name[1] == '<' && name[2] == '\0') //detects exacly the '<<'
		return (TKN_HEREDOC);
	else if (ft_strcmp(name, ">>") == 0)
		return (TKN_APPEND);
	else if (name[0] == '>')
		return (TKN_OUTREDIR);
	else if (name[0] == '<')
		return (TKN_INREDIR);
	else if (i == 0 || msh->data->args[i-1][0] == '|')	// (i == 0 || args[i-1] == TKN_PIPE)
		return (TKN_BCMD);
	else if (name[0] == ' ')
		return (TKN_SPACE);
	else
		return (ARGUMENT);
}

int	get_type(t_msh *msh, char *name, int i)
{
	if (get_builtin_type(name) != 0) // Check if it is a builtin cmd
		return (get_builtin_type(name));
	else if (get_meta_type(msh, name, i) != 0)	// check for metachars
		return (get_meta_type(msh, name, i));
	else
		return (1);
}
