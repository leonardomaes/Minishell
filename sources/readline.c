/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/25 19:03:46 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *prompt()
{
	char *line;

	line = readline(" % ");
	if (!line)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

t_data  *ft_readline(void)
{
	t_data *data;
	char	*line;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	line = prompt();		// Le o input
	// Start parser
	return (data);
}	