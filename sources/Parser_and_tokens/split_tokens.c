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

void	add_one(char ***str, const char *s, int *i, int *j, int *l)
{
	(*str)[*i][(*j)++] = s[(*l)++];
	(*str)[*i][*j] = '\0';
}

void	split_redirs(char ***str, const char *s, int *i, int *j, int *l)
{
	while (ft_isredirection(s[*l]))
		(*str)[*i][(*j)++] = s[(*l)++];
	(*str)[*i][*j] = '\0';
}

void	split_else(char ***str, const char *s, int *i, int *j, int *l)
{
	while (s[*l] && s[*l] != '|' && !ft_isspace(s[*l]) && !ft_isdelimiter(s[*l]) && !ft_isredirection(s[*l]))
		(*str)[*i][(*j)++] = s[(*l)++];
	(*str)[*i][*j] = '\0';
}

void	split_arg(t_msh *msh, char ***str, const char *s, int *l, int *i)
{
	int	j;

	j = 0;
	if (ft_isspace(s[*l]) && (*i == 0 || s[*l - 1] == '|'))
		skip_spaces(s, l);
	if (ft_isspace(s[*l]))
	{
		add_one(str, s, i, &j, l);
		skip_spaces(s, l);
	}
	else if (s[*l] == '|')
		add_one(str, s, i, &j, l);
	else if (s[*l] == '"')
		j = handle_double_quote(msh, s, (*str)[*i], l);
	else if (s[*l] == '\'')
		j = handle_single_quote(s, (*str)[*i], l);
	else if (s[*l] == '$' && !ft_isdelimiter(s[*l+1]) && !ft_isspace(s[*l+1]))
		j = handle_environ(msh, s, (*str)[*i], l);
	else if (s[*l] == '$' && s[*l+1] == '\0')
		add_one(str, s, i, &j, l);
	else if (s[*l] && ft_isredirection(s[*l]))
		split_redirs(str, s, i, &j, l);
	else
		split_else(str, s, i, &j, l);
	(*i)++;
}

char **ft_split_args(t_msh *msh, const char *s)
{
	char	**str;
	int		*len;
	int		words;
	int		i;
	int		l;

	if (!s)
		return (NULL);
	words = count_args(s);
	len = calculate_lengths(msh, s, words);
	if (!len)
		return (NULL);
	str = alloc_args(words, len);
	if (!str)
	{
		free(len);
		return (NULL);
	}
	i = 0;
	l = 0;
	while (s[l])
		split_arg(msh, &str, s, &l, &i);
	str[i] = NULL;
	free(len);
	return (str);
}
