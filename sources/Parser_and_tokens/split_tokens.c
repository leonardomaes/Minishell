/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
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
		/* while (s[i] && ft_isspace(s[i]))
			i++; */
		if (s[i] == '\0')
			break;
		word++;
		if (ft_isspace(s[i]) && (word == 1 || s[i-1] == '|' )) // Trocar para identificar após primeiro argumento
		{
			while (ft_isspace(s[i]))
				i++;
		}
		if (ft_isspace(s[i]))
		{
			//printf("-%c\n", s[i-1]);
			while (ft_isspace(s[i]) )
				i++;
		}
		else if (s[i] == '|')
		{
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
		else if (s[i] == '$' && s[i+1] == '\0')
		{
			i++;
		}
		
		else if (ft_isredirection(s[i]))
		{
			while (ft_isredirection(s[i]))
				i++;
		}
		else
		{
			while (!ft_isspace(s[i]) && s[i] != '|' && s[i] && !ft_isdelimiter(s[i]) && !ft_isredirection(s[i]))
				i++;
		}
	}
	return (word);
}

int	*calculate_lengths(t_msh *msh, const char *s, int words) // Calcula tamanho dos argumentos para alocar
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
		if (ft_isspace(s[i]) && (word == 0 || s[i-1] == '|'))
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
		else if (s[i] == '|')
		{
			len[word] = 1;
			i++;
		}
		else if (s[i] == '$' && !ft_isdelimiter(s[i+1]) && !ft_isspace(s[i+1])) // Dolar
			len[word] = environ_lenght(msh, s, &i);
		else if (s[i] == '$' && s[i+1] == '\0')
		{
			len[word] = 1;
			i++;
		}
		else if (s[i] == '\'')
			len[word] = single_quote_lenght(s, &i);
		else if (s[i] == '"')
			len[word] = double_quote_lenght(msh, s, &i);
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
			while (s[i] && s[i] != '|' && !ft_isspace(s[i]) && !ft_isdelimiter(s[i]) && !ft_isredirection(s[i]))
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

char **ft_split_args(t_msh *msh, const char *s)
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
	len = calculate_lengths(msh, s, words);
	if (!len)
		return (NULL);
	str = alloc_args(words, len);
	if (!str)
		return (NULL);
	i = 0;
	l = 0;
	while (s[l])
	{
		j = 0;
		if (s[l] == '\0')
			break;
		if (ft_isspace(s[l]) && (i == 0 || s[l - 1] == '|'))
		{
			while (ft_isspace(s[l]))
				l++;
		}
		if (ft_isspace(s[l])) // Se for espaços multiplos, adiciona apenas um espaço
		{
			str[i][j++] = s[l++];
			str[i][j] = '\0';
			while (ft_isspace(s[l]))
				l++;
		}
		else if (s[l] == '|')
		{
			str[i][j++] = s[l++];
			str[i][j] = '\0';
		}
		else if (s[l] == '"')
			j = handle_double_quote(msh, s, str[i], &l);
		else if (s[l] == '\'')
			j = handle_single_quote(s, str[i], &l);
		else if (s[l] == '$' && !ft_isdelimiter(s[l+1]) && !ft_isspace(s[l+1]))
			j = handle_environ(msh, s, str[i], &l);
		else if (s[l] == '$' && s[l+1] == '\0')
		{
			str[i][j++] = s[l++];
			str[i][j] = '\0';
		}
		else if (s[l] && ft_isredirection(s[l]))
		{
			while (ft_isredirection(s[l]))
				str[i][j++] = s[l++];
			str[i][j] = '\0';
		}
		else
		{
			while (s[l] && s[l] != '|' && !ft_isspace(s[l]) && !ft_isdelimiter(s[l]) && !ft_isredirection(s[l])) // don't add redirs to args
				str[i][j++] = s[l++];
			str[i][j] = '\0';
		}
		i++;
	}
	str[i] = NULL;
	free(len);
	return (str);
}
