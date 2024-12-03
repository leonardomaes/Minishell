/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:39 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_countargs(char **args)
{
	int i;
	i = 0;
	while (args[i])
		i++;
	return (i);	
}

int ft_parsing(t_msh *msh, char *line)
{
	int i;
	i = 0;

	msh->data->args = ft_split_args(line);				// Split dos argumentos em vetor de strings
	if (!msh->data->args || !msh->data->args[0] || *msh->data->args[0] == '\0')
		return (1);
	
	msh->data->argc = ft_countargs(msh->data->args);	// Lê a quantidade de args
	split_tokens(msh, &msh->data->tokens, NULL, i);			// Passa os parametros para structs de tokens
	// Syntax check	
	return (0);
}
