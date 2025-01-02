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

int	count_args(const char *s) // Conta quantidade de argumentos
{
	int	i;
	int	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]))
			i++;
		if (s[i] == '\0')
			break;
		word++;
		if (s[i] == '"')
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
		else if (s[i] == '$')
		{
			i++;
			while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
				i++;
		}
		else
		{
			while (!ft_isspace(s[i]) && s[i])
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
		while (s[i] && ft_isspace(s[i]))
			i++;
		if (s[i] == '\0')
			break;
		if (s[i] == '$')
			len[word] = environ_lenght(s, &i);
		else if (s[i] == '\'')
			len[word] = single_quote_lenght(s, &i);
		else if (s[i] == '"')
			len[word] = double_quote_lenght(s, &i);
		else
		{
			while (s[i] && !ft_isspace(s[i]))
			{
				len[word]++;
				i++;
			}
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

char	**ft_split_args(const char *s) // Take the line and transform it in a array of strings
{
	char 	**str;
	int		*len;
	int		words;
	int		i;
	int		j;

	len = NULL;
	words = count_args(s);
	if (!s || words < 0)
		return (NULL);
	len = calculate_lengths(s, words);
	if (!len)
		return (NULL);
	str = alloc_args(words, len);
	if (!str)
		return (NULL);
	i = 0;
	while (*s)
	{
		j = 0;
		while (*s && ft_isspace(*s))
			s++;
		if (*s == '\0')
			break ;
		if (*s == '"')
			j = handle_double_quote(&s, str[i]);
		else if(*s == '\'')
			j = handle_single_quote(&s, str[i]);
		else if (*s == '$')
			j = handle_environ(&s, str[i]);
		else
		{
			while (*s && !ft_isspace(*s))
				str[i][j++] = *s++;
			str[i][j] = '\0';
		}
		i++;
	}
	str[i] = NULL;
	free(len);
	return (str);
}

char **get_args(char **data_args, int i, t_msh *msh)
{
	int k;
	int j;
	char **args;

	j = i;
	while (data_args[j] && get_type(data_args[j]) != TKN_PIPE)
	j++;
	args = malloc(sizeof(char *) * (j - i + 1));
	if (!args)
	{
		perror("malloc:");
		ft_free_all(msh);
		exit(1);
	}
	k = 0;
	j = i;
	while (data_args[j] && get_type(data_args[j]) != TKN_PIPE)
		args[k++] = ft_strdup(data_args[j++]);
	args[k] = NULL;
	return (args);
}

void	split_tokens(t_msh *msh, t_tokens **token, t_tokens *prev, int i)	// Pass agrs to linked list
{
	t_tokens	*temp;

	if (i < msh->data->argc)
	{
		*token = NULL;
		*token = malloc(sizeof(t_tokens));
		temp = *token;
		temp->name = msh->data->args[i];
		temp->type = get_type(msh->data->args[i]);
		if (temp->type == TKN_PIPE)
			msh->data->pipes++;
		temp->count = msh->data->pipes;
		//temp->count = i;
		if (prev != NULL)
			temp->prev = prev;
		if (prev == NULL || temp->prev->type == TKN_PIPE) // Adiciona o comando para o primeiro token de cada comando
			temp->args = get_args(msh->data->args, i, msh);
		else
			temp->args = NULL;
		temp->next = NULL;
		i++;
		split_tokens(msh, &temp->next, temp, i);
	}
}

