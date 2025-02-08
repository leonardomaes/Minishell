/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/09 18:02:56 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	ft_countargs(char **args)
{
	int i;
	i = 0;
	while (args[i])
		i++;
	return (i);	
}

int	get_redir(t_tokens *temp)
{
	if (temp->type == TKN_APPEND || temp->type == TKN_INREDIR || temp->type == TKN_OUTREDIR)
		return (1);
	return (0);
}

int syntax_redirs(t_msh *msh, t_tokens *tokens)
{
	t_tokens	*temp;

	temp = tokens;
	while (temp)
	{
		if (get_redir(temp) == 1)
			break;
		temp = temp->next;
	}
	if (!temp)
		return (0);
	if (temp->next == NULL)
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		g_exit = 2;
		return (1);
	}
	if (get_delimiter(msh, temp->next->name) != 0) // Arrumar esta verificação
	{
		printf("bash: syntax error near unexpected token '%s'\n", temp->next->name);
		g_exit = 2;
		return (1);
	}
	return (0);
}

int	syntax_pipes(t_tokens *tokens)
{
	t_tokens *temp;

	temp = tokens;
	while (temp)
	{
		if (temp->type == TKN_PIPE && (temp->next == NULL || temp->next->type == TKN_PIPE))
		{
			printf("bash: syntax error near unexpected token `%s'\n", temp->name);
			return (1);
		}
		if (temp->type == TKN_PIPE && temp->next == NULL)
		{
			printf("bash: syntax error near unexpected token `%s'\n", temp->name);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

int	syntax_quotes(t_tokens *tokens)
{
	t_tokens *temp;
	int			i;

	temp = tokens;
	while (temp)
	{
		if (temp->type == DBL_QUOTES)
		{
			i = 0;
			while (temp->name[i])
			{
				if (temp->name[i+1] == '"')
					break;
				else if (temp->name[i+1] == '\0')
					return (1);
				i++;
			}
		}
		else if (temp->type == SNG_QUOTES)
		{
			i = 0;
			while (temp->name[i])
			{
				if (temp->name[i+1] == '\'')
					break;
				else if (temp->name[i+1] == '\0')
					return (1);
				i++;
			}
		}
		temp = temp->next;
	}
	return (0);
}

int	syntax_check(t_msh *msh, t_data *data)
{
	if (data->tokens->type == TKN_PIPE)
	{
		printf("bash: syntax error near unexpected token `%s'\n", data->tokens->name);
		g_exit = 2;
		return (1);
	}
	if (data->tokens->type == TKN_HEREDOC)
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		g_exit = 2;
		return (1);
	}
	if (syntax_quotes(data->tokens))
	{
		printf("bash: syntax error near unexpected token 'open quote'\n");
		g_exit = 2;
		return (1);
	}
	if (syntax_pipes(data->tokens) != 0)
		return (g_exit = 2, 1);
	if (syntax_redirs(msh, data->tokens) != 0)
		return (g_exit = 2, 1);
	return (0);
}

int	ft_init_data(char *line, t_msh *msh)
{
	int i;

	i = 0;
	msh->data->cmd_paths = NULL;
	msh->data->cmd_paths = ft_split(ft_get_path((*msh).envp), ':');
	msh->data->args = ft_split_args(msh, line);
	if (!msh->data->args || !msh->data->args[0] || (*msh->data->args[0] == '\0' && !msh->data->args[1]))
	{
		if (msh->data->args)
		{
			while (msh->data->args[i])
				free(msh->data->args[i++]);
			free(msh->data->args);
		}
		return (1);
	}
	msh->data->argc = ft_countargs(msh->data->args);		// Lê a quantidade de args
	msh->data->pipes = 0;
	msh->data->infile = -2;
	msh->data->outfile = -2;
	return (0);
}

int	ft_readline(t_msh *msh)
{
	char	*line;
	int		i;

	i = 0;
	msh->data = malloc(sizeof(t_data));
	if (!msh->data)
		return (1);
	line = ft_prompt();									// Lê o input
	if (!line) //RM: included this because noticed missing EOF handling (Ctrl+D)
	{
		free(msh->data); //RM: this can cause SEGFAULT if we do not adapt the ft_free_all function (made a change proposal)
		msh->data = NULL;
		return (1);
	}
	else if (*line == '\0')	// Erro ao enviar readline com string vazia, precisa corrigir
	{
		free(msh->data);
		msh->data = NULL;
		free(line);
		return (0);
	}
	if (ft_init_data(line, msh) != 0) 			// Inicia struct s_data
		return (free(msh->data), free(line), 1);
	//ft_print_params(msh); 	// Remover
	split_tokens(msh, &msh->data->tokens, NULL, i);		// Passa os parametros para structs de tokens
	//ft_print_tokens(msh->data->tokens); 	// Remover
	if (syntax_check(msh, msh->data) != 0)					// Verificaçao de sintaxe
		return (ft_free_data(msh), free(line), 1); // Erro aqui quando return == 1, ocorre varios leaks
	//ft_print_params(msh); 	// Remover
	//ft_print_tokens(msh->data->tokens); 	// Remover
	//printf("\n<--------------------------------->\n"); 	// Remover
	free(line);
	return (0);
}
