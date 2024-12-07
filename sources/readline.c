/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/07 19:11:47 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_prompt()
{
	char *line;

	line = readline("\001\033[1;36m\002cmd>\001\033[0m\002");
	if (!line)
	{
//		free(line);      //RM: validate do eliminate this free with Leo
		return (NULL);
	}
	else if (*line != '\0')
		add_history(line);								// Add ao historico
	return (line);
}

void	ft_readline(t_msh *msh)
{
	char	*line;

	msh->data = malloc(sizeof(t_data));
	if (!msh->data)
		return ;
	printf("<--------------------------------->\n"); 	// Remover
	line = ft_prompt();									// Lê o input
	if (!line) //RM: included this because noticed missing EOF handling
	{
		free(msh->data); //THIS IS CAUSING A SEGFAULT !!!
		msh->data = NULL;
		return ;
	}

	if (ft_parsing(msh, line) != 0)						// Comeca a tratar os dados
		return free(line);

	ft_print_params(msh); 	// Remover
	ft_print_tokens(msh); 	// Remover

	// Ler quantidade de pipes e dividir os tokens
	// Start parser
	// Talvez fazer tokens diferentes, para casos:
	// Sem pipe - Sem input/output - Com pipe - Com input/output
	free(line);
}
