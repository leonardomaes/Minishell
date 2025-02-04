/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:39 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_isdelimiter(char c)
{
	return (c == '"' || c == '\'' || c == '\0');
}

int	ft_isredirection(char c)
{
	return (c == '<' || c == '>');
}

//printf("1->%s\n", s + 2);	// String a partir da posição
//printf("2->%c\n", *s + 2);	// Caractere + 2 ASCII
//printf("3->%s\n", &s[2]);	// String a partir da posição
//printf("4->%c\n", s[2]);	// Caractere da posição


/* char **ft_merge_args(char **args)
{
	char **joined_args;
	int	i;
	int	j;
	int	k;

	i = 0;
	while (args[i]) // Each index is a arg
	{
		if (args[i][0] == ' ' || (args[i+1] == NULL && args[i][0] != ' '))// Each j is a joined arg
			j++;
		i++;
	}
	new_args = (char **)malloc(sizeof(char *) * (j + 1));
	i = 0;
	while (i < j)
	{
		k = 0;
		while (args[i][0] != ' ')
			k += ft_strlen(args[i++]);
		i++;
		//joined_args[i] = (char *)malloc(sizeof(char) * (ft_strlen(args[j]) + 1))
	}
	joined_args[i] = NULL;
	return(joined_args);
} */
int get_delimiter(char *data_args)
{
	if (get_type(data_args, 1) == TKN_PIPE)
		return (TKN_PIPE);
	else if(get_type(data_args, 1) == TKN_INREDIR)
		return (TKN_INREDIR);
	else if(get_type(data_args, 1) == TKN_OUTREDIR)
		return (TKN_OUTREDIR);
	else if(get_type(data_args, 1) == TKN_APPEND)
		return (TKN_APPEND);
	else if (get_meta_type(data_args, 1) == TKN_HEREDOC)
		return (TKN_HEREDOC);
	return (0);
}
/* char	**get_args(char **data_args, int i, t_msh *msh)
{
	int		k;
	int		j;
	char	**args;

	//count the number of args until we get to a pipe or end
	j = i;
	k = 0;
	while (data_args[j] && data_args[j][0] != '|') // Ciclo para calcular alocação
	{
		if (get_delimiter(data_args[j]) != 0) // Caso seja um redirect ou pipe
		{
			j++;
			if (data_args[j][0] == ' ') // Avança o primeiro espaço logo após o redir ou pipe
				j++;
			while (data_args[j] && data_args[j][0] != ' ' && data_args[j][0] != '|') // avança o argumento inteiro até o espaço após
				j++;
			continue;
		}
		if (data_args[j][0] != ' ') // copia se não for espaço
			k++;
		j++;
	}
	//allocate space for keeping the args and NULL
	args = malloc(sizeof(char *) * (k + 1));
	if (!args)
	{
		perror("malloc:");
		ft_free_all(msh);
		exit(1);
	}
	//copy all tokens
	k = 0;
	while (data_args[i] && i < j)
	{
		if (get_delimiter(data_args[i]) != 0) // Caso seja um redirect ou pipe
		{
			i++;
			if (data_args[i][0] == ' ') // Avança o primeiro espaço logo após o redir ou pipe
				i++;
			while (data_args[i] && data_args[i][0] != ' ' && data_args[i][0] != '|') // avança o argumento inteiro até o espaço após
				i++;
			continue;
		}
		//printf("-%s\n", data_args[i]);
		if (data_args[i][0] == '"')
			args[k++] = ft_chartrim(&data_args[i], '"'); // Copia sem aspas duplas
		else if (data_args[i][0] == '\'')
			args[k++] = ft_chartrim(&data_args[i], '\''); // Copia sem aspas simples
		else if (data_args[i][0] != ' ')
			args[k++] = ft_strdup(data_args[i]);
		//free previously allocated memory in case strdup fails
		if (!args[k-1])
		{
			perror("strdup");
			while (k >= 0)
				free(args[k--]);
			free(args);
			ft_free_all(msh);
			exit(1);
		}
		i++;
	}
	args[k] = NULL;
	return (args);
} */
char	**get_args(char **data_args, int i, t_msh *msh)
{
	int		k;	// Argumentos merged
	int		j;	// Qtd args até pipe ou final
	char	**args;
	char	*merged;
	char	*temp;
	char	*next_arg;

	//count the number of args until we get to a pipe or end
	j = i;
	k = 0;
	while (data_args[j] && data_args[j][0] != '|') // Ciclo para calcular alocação
	{
		if (get_delimiter(data_args[j]) != 0) // Caso seja um redirect ou pipe
		{
			j++;
			if (data_args[j][0] == ' ') // Avança o primeiro espaço logo após o redir ou pipe
				j++;
			while (data_args[j] && data_args[j][0] != ' ' && data_args[j][0] != '|') // avança o argumento inteiro até o espaço após
				j++;
			continue;
		}
		if (data_args[j][0] == ' ' || data_args[j+1])
			k++;
		j++;
	}
	if (k == 0)
		k++;
	//allocate space for keeping the args and NULL
	args = malloc(sizeof(char *) * (k + 1));
	if (!args)
	{
		perror("malloc:");
		ft_free_all(msh);
		exit(1);
	}
	//copy all tokens
	k = 0;
	while (data_args[i] && data_args[i][0] != '|' && i < j)	// Enquanto não chegar no pipe ou no final
	{
		if (get_delimiter(data_args[i]) != 0) // Caso seja um redirect ou pipe, apenas avança
		{
			i++;
			if (data_args[i][0] == ' ') // Avança o primeiro espaço logo após o redir ou pipe
				i++;
			while (data_args[i] && data_args[i][0] != ' ' && data_args[i][0] != '|') // avança o argumento inteiro até o espaço após
				i++;
			continue;
		}
		else if (data_args[i][0] != ' ')
		{
			if (data_args[i][0] == '"')
				merged = ft_chartrim(&data_args[i], '"');
			else if (data_args[i][0] == '\'')
				merged = ft_chartrim(&data_args[i], '\'');
			else
				merged = ft_strdup(data_args[i]);
			i++;
			while (data_args[i] && data_args[i][0] != ' ')
			{
				if (data_args[i][0] == '"')
					next_arg = ft_chartrim(&data_args[i], '"');
				else if (data_args[i][0] == '\'')
					next_arg = ft_chartrim(&data_args[i], '\'');
				else
					next_arg = ft_strdup(data_args[i]);
				temp = merged;
				merged = ft_strjoin(merged, next_arg);
				free(temp);
				free(next_arg);
				i++;
			}
			args[k++] = merged;
		}
		else
			i++;
		//free previously allocated memory in case strdup fails
		/* if (!args[k])	//Alterar para k-1
		{
			perror("strdup");
			while (k >= 0)
				free(args[k--]);
			free(args);
			ft_free_all(msh);
			exit(1);
		} */
		// i++;
	}
	args[k] = NULL;
	return (args);
}

void	split_tokens(t_msh *msh, t_tokens **token, t_tokens *prev, int i)	// Pass agrs to linked list
{
	t_tokens	*temp;

	if (i < msh->data->argc)
	{
		if (!msh || !msh->data)
			return ;
		*token = NULL;
		*token = malloc(sizeof(t_tokens));
		if (!*token)
		{
			g_exit = 1;
			return ;
		}
		temp = *token;
		temp->name = msh->data->args[i];
		if (prev == NULL || prev->type == TKN_PIPE)
			temp->type = get_type(msh->data->args[i], i);
		else // Somente entra em get_type se for primeiro argumento
			temp->type = get_meta_type(msh->data->args[i], 1);
		// Handle heredoc case
		if (temp->type == TKN_PIPE) //Se for pipe incrementa soma
			msh->data->pipes++;
		temp->count = msh->data->pipes; // Contagem do token no conjunto
		if (prev != NULL)				// Aponta para o token anterior
			temp->prev = prev;
		if (temp->type == TKN_HEREDOC)	// Heredocs configs
		{
			/* if (i + 1 >= msh->data->argc || get_type(msh->data->args[i + 1], 1) != ARGUMENT)
			{
				ft_putstr_fd("syntax error near unexpected token 'newline'\n", 2);
				g_exit = 2;
				free(*token);
				*token = NULL;
				return ;
			} */
			temp->args = malloc(sizeof(char *) * 2);
			if (!temp->args)
			{
				free(*token);
				*token = NULL;
				return ;
			}
			if (msh->data->args[i + 1][0] == ' ')
				i++;
			if (msh->data->args[i+1][0] == '"')
				temp->args[0] = ft_chartrim(&msh->data->args[i + 1], '"'); //store the delimiter like 'EOF'
			else if (msh->data->args[i+1][0] == '\'')
				temp->args[0] = ft_chartrim(&msh->data->args[i + 1], '\'');
			else
				temp->args[0] = ft_strdup(msh->data->args[i+1]);
			temp->args[1] = NULL;
			//i++;  // Skip the delimiter in the main array
		}//if we are at the start of a command or after a pipe get all args
		else if (prev == NULL || temp->prev->type == TKN_PIPE) // Adiciona o comando para o primeiro token de cada comando
		{
			//temp->name = ft_chartrim(&temp->name, ' ');  // Esta causando o leak
			temp->args = get_args(msh->data->args, i, msh);
			if (!temp->args)
			{
				free(*token);
				*token = NULL;
				return;
			}
		}
		else // Possivel erro aqui
			temp->args = NULL;
		temp->next = NULL;
		i++;
		split_tokens(msh, &temp->next, temp, i);
	}
}
