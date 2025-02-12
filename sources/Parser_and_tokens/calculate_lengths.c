/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_lengths.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:39 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	calculate_quotes(t_msh *msh, const char *s, int word, int *i, int **len)
{
	if (s[*i] == '\'')
		(*len)[word] = single_quote_lenght(s, i);
	else if (s[*i] == '"')
		(*len)[word] = double_quote_lenght(msh, s, i);
}

void	calculate_redir(const char *s, int word, int *i, int **len)
{
	while (ft_isredirection(s[*i]))
	{
		(*len)[word]++;
		(*i)++;
	}
}

void	sum_one(int word, int *i, int **len)
{
	(*len)[word] = 1;
	(*i)++;
}

void	calculate_length(t_msh *msh, const char *s, int word, int *i, int **len)
{
	if (ft_isspace(s[*i]))
	{
		(*len)[word] = 1;
		skip_spaces(s, i);
	}
	else if (s[*i] == '|')
		sum_one(word, i, len);
	else if (s[*i] == '$' && !ft_isdelimiter(s[*i + 1]) && !ft_isspace(s[*i + 1]))
		(*len)[word] = environ_lenght(msh, s, i);
	else if (s[*i] == '$' && s[*i + 1] == '\0')
		sum_one(word, i, len);
	else if (s[*i] == '\'' || s[*i] == '"')
		calculate_quotes(msh, s, word, i, len);
	else if (ft_isredirection(s[*i]))
		calculate_redir(s, word, i, len);
	else
	{
		while (s[*i] && s[*i] != '|' && !ft_isspace(s[*i]) && !ft_isdelimiter(s[*i]) && !ft_isredirection(s[*i]))
		{
			(*len)[word]++;
			(*i)++;
		}
	}
}

int	*calculate_lengths(t_msh *msh, const char *s, int words)
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
		if (s[i] == '\0')
			break;
		if (ft_isspace(s[i]) && (word == 0 || s[i-1] == '|'))
			skip_spaces(s, &i);
		calculate_length(msh, s, word, &i, &len);
		word++;
	}
	return (len);
}
