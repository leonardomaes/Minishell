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

char *ft_prompt()
{
	char *line;

	line = readline("\001\033[1;36m\002cmd>\001\033[0m\002");
	if (!line)
	{
		free(line);
		return (NULL);
	}
	else if (*line != '\0')
		add_history(line);								// Add ao historico
	return (line);
}

int	ft_countargs(char **args)
{
	int i;
	i = 0;
	while (args[i])
		i++;
	return (i);	
}

int	syntax_check(t_data *data)
{
	if (data->tokens->type == TKN_PIPE)
	{
		printf("bash: syntax error near unexpected token `%s'\n", data->tokens->name);
		return (1);
	}
	
	return (0);
}

void	ft_readline(t_msh *msh)
{
	char	*line;
	int		i;

	i = 0;
	msh->data = malloc(sizeof(t_data));
	if (!msh->data)
		return ;
	line = ft_prompt();										// Lê o input
	msh->data->args = ft_split_args(line);					// Faz split em vetores
	if (!msh->data->args || !msh->data->args[0] || *msh->data->args[0] == '\0')
		return free(line);
	msh->data->argc = ft_countargs(msh->data->args);		// Lê a quantidade de args
	split_tokens(msh, &msh->data->tokens, NULL, i);			// Passa os parametros para structs de tokens
	if (syntax_check(msh->data) != 0)						// Verificaçao de sintaxe
		return free(line);
	
	//printf("\n%s\n", expand_env(msh->envp, "PATH"));
	ft_print_params(msh); 	// Remover
	ft_print_tokens(msh); 	// Remover
	printf("<--------------------------------->\n"); 	// Remover

	// Ler quantidade de pipes e dividir os tokens
	// Start parser
	// Talvez fazer tokens diferentes, para casos:
	// Sem pipe - Sem input/output - Com pipe - Com input/output
	free(line);
}
