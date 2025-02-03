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

int	count_args(const char *s) // Conta quantidade de argumentos
{
	int	i;
	int	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		/* while (s[i] && ft_isspace(s[i]))
			i++; */
		if (s[i] == '\0')
			break;
		word++;
		if (ft_isspace(s[i]) && (s[i-1] == '|' || word == 2 )) // Trocar para identificar após primeiro argumento
		{
			while (ft_isspace(s[i]))
				i++;
		}
		if (ft_isspace(s[i]))
		{
			//printf("-%c\n", s[i-1]);
			while (ft_isspace(s[i]))
				i++;
		}
		else if (s[i] == '"')
		{
			i++;
			while (s[i] && s[i] != '"')
				i++;
			if (s[i] == '"')
				i++;
		}
		else if (s[i] == '\'')
		{
			i++;
			while (s[i] && s[i] != '\'')
				i++;
			if (s[i] == '\'')
				i++;
		}
		else if (s[i] == '$' && !ft_isdelimiter(s[i+1]) && !ft_isspace(s[i+1])) // Dolar
		{
			i++;
			if (s[i] == '?')
				i++;
			else if (ft_isdelimiter(s[i]) == 0)
			{
				while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
					i++;
			}
		}
		else if (ft_isredirection(s[i]))
		{
			while (ft_isredirection(s[i]))
				i++;
		}
		else
		{
			while (!ft_isspace(s[i]) && s[i] && !ft_isdelimiter(s[i]))
				i++;
		}
	}
	return (word);
}

int	*calculate_lengths(const char *s, int words) // Calcula tamanho dos argumentos para alocar
{
	int *len;
	int i;
	int word;

	i = 0;
	word = 0;
	len = (int *)malloc(sizeof(int) * words);
	if (!len)
		return NULL;
	while (s[i] && word < words)
	{
		len[word] = 0;
		/* while (s[i] && ft_isspace(s[i]))
			i++; */
		if (s[i] == '\0')
			break;
		if (ft_isspace(s[i]) && (s[i-1] == '|' || word == 1))
		{
			while (ft_isspace(s[i]))
				i++;
		}
		if (ft_isspace(s[i]))
		{
			//printf("-%c\n", s[i-1]);
			len[word] = 1;
			while (ft_isspace(s[i]))
				i++;
		}
		else if (s[i] == '$' && !ft_isdelimiter(s[i+1]) && !ft_isspace(s[i+1])) // Dolar
			len[word] = environ_lenght(s, &i);
		else if (s[i] == '\'')
			len[word] = single_quote_lenght(s, &i);
		else if (s[i] == '"')
			len[word] = double_quote_lenght(s, &i);
		else if (ft_isredirection(s[i]))
		{
			while (ft_isredirection(s[i]))
			{
				len[word]++;
				i++;
			}
		}
		else
		{
			while (s[i] && !ft_isspace(s[i]) && !ft_isdelimiter(s[i]))
			{
				len[word]++;
				i++;
			}
			/* if (ft_isspace(s[i]))
			{
				len[word]++;
				i++;
			} */
		}
		word++;
	}
	return (len);
}

char **alloc_args(int words, int *len) // Allocations for array of strings
{
	char **str;
	int i;
	
	str = (char **)malloc(sizeof(char *) * (words + 1));
	i = 0;
	while (i < words)
	{
		str[i] = (char *)malloc(sizeof(char) * (len[i] + 1));
		if (!str[i])
		{
			free_array(str, i);
			free(len);
			return (NULL);
		}
		i++;
	}
	str[i] = NULL;
	return (str);
}

//printf("1->%s\n", s + 2);	// String a partir da posição
//printf("2->%c\n", *s + 2);	// Caractere + 2 ASCII
//printf("3->%s\n", &s[2]);	// String a partir da posição
//printf("4->%c\n", s[2]);	// Caractere da posição

char **ft_split_args(const char *s)
{
	char    **str;
	int     *len;
	int     words;
	int     i;
	int     j;
	int     l;

	if (!s)
		return (NULL);
	words = count_args(s);
	len = calculate_lengths(s, words);
	if (!len)
		return (NULL);
	str = alloc_args(words, len);
	if (!str)
		return (NULL);
	i = 0;
	l = 0;
	while (s[l])
	{
		//printf("%c-", s[l]);
		j = 0;
		/* while (s[l] && ft_isspace(s[l]))
			l++; */
		if (s[l] == '\0')
			break;
		if (ft_isspace(s[l]) && (s[l-1] == '|' || i == 1))
		{
			while (ft_isspace(s[l]))
				l++;
			//i++;
		}
		if (ft_isspace(s[l])) // leak aqui
		{
			//printf("-%c\n", s[l-1]);
			str[i][j++] = s[l++];
			str[i][j] = '\0';
			while (ft_isspace(s[l]))
				l++;
		}
		else if (s[l] == '"')
			j = handle_double_quote(s, str[i], &l);
		else if (s[l] == '\'')
			j = handle_single_quote(s, str[i], &l);
		else if (s[l] == '$' && !ft_isdelimiter(s[l+1]) && !ft_isspace(s[l+1]))
			j = handle_environ(s, str[i], &l);
		else if (s[l] && ft_isredirection(s[l]))
		{
			while (ft_isredirection(s[l]))
				str[i][j++] = s[l++];
			str[i][j] = '\0';
		}
		else
		{
			while (s[l] && !ft_isspace(s[l]) && !ft_isdelimiter(s[l]))
				str[i][j++] = s[l++];
			str[i][j] = '\0';
		}
		i++;
	}
	str[i] = NULL;
	free(len);
	return (str);
}

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

char	**get_args(char **data_args, int i, t_msh *msh)
{
	int		k;
	int		j;
	char	**args;

	//count the number of args until we get to a pipe or end
	j = i;
	k = 0;
	while (data_args[j] && data_args[j][0] != '|') // Ciclo para calcular alocação
	{
		if (get_delimiter(data_args[j]) != 0) // Caso seja um redirect
		{
			j++;
			if (data_args[j][0] == ' ') // Avança o primeiro espaço se tiver um
				j++;
			while (data_args[j] && data_args[j][0] != ' ' && data_args[j][0] != '|') // Copia até achar um espaço ou pipe
				j++;
			continue;
		}
		if (data_args[j][0] != ' ')
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
		if (get_delimiter(data_args[i]) != 0)
		{
			i++;
			if (data_args[i][0] == ' ')
				i++;
			while (data_args[i] && data_args[i][0] != ' ' && data_args[i][0] != '|')
				i++;
			continue;
		}
		//printf("-%s\n", data_args[i]);
		if (data_args[i][0] == '"')
			args[k++] = ft_chartrim(&data_args[i], '"');
		else if (data_args[i][0] == '\'')
			args[k++] = ft_chartrim(&data_args[i], '\'');
		else if (data_args[i][0] != ' ')
			args[k++] = ft_strdup(data_args[i]);
		//free previously allocated memory in case strdup fails
		/* if (!args[k])
		{
			perror("strdup");
			while (k >= 0)
				free(args[k--]);
			free(args);
			ft_free_all(msh);
			exit(1);
		} */
		i++;
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
			if (msh->data->args[i+1][0] == '"')
				temp->args[0] = ft_chartrim(&msh->data->args[i + 1], '"'); //store the delimiter like 'EOF'
			else
				temp->args[0] = ft_chartrim(&msh->data->args[i + 1], '\'');
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
