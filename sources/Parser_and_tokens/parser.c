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

void ft_parsing(t_msh *msh)
{
	int i;
	i = 0;
	msh->data->argc = ft_countargs(msh->data->args);	// Lê a quantidade de args
	split_tokens(msh, &msh->data->tokens, i);			// Passa os parametros para structs de tokens
}


/*
ECHO:
echo [option] [string]
echo [string]

flag:
-n -> print without a new line after
-e -> Habilita interpretaçao da contrabarra
\b -> remove spaces in between the text
\c -> omitted trailing new line
\n -> create a new line where it is used
\t -> horizontal tab spaces
\r -> text before \r is not printed
\v -> vertical tabs
\a -> will produce an alert sound

"echo *" is similar to ls command

echo "Hello World" > output.txt
">" to overwrite
">>" to append

*/