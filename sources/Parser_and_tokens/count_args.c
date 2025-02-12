/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:39 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	skip_spaces(const char *s, int *i)
{
	while (ft_isspace(s[*i]))
	{
		(*i)++;
	}
}

void	count_dollar(const char *s, int *i)
{
	if (s[*i] == '$' && !ft_isdelimiter(s[*i+1]) && !ft_isspace(s[*i+1]))
	{
		(*i)++;
		if (s[*i] == '?')
			(*i)++;
		else if (ft_isdelimiter(s[*i]) == 0)
		{
			while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
				(*i)++;
		}
	}
	else if (s[*i] == '$' && s[*i + 1] == '\0')
	{
		(*i)++;
	}
}

void	count_quotes(const char *s, int *i)
{
	if (s[*i] == '"')
	{
		(*i)++;
		while (s[*i] && s[*i] != '"')
			(*i)++;
		if (s[*i] == '"')
			(*i)++;
	}
	else if (s[*i] == '\'')
	{
		(*i)++;
		while (s[*i] && s[*i] != '\'')
			(*i)++;
		if (s[*i] == '\'')
			(*i)++;
	}
	
}

void	count_arg(const char *s, int *i)
{
	if (ft_isspace(s[*i]))
		skip_spaces(s, i);
	else if (s[*i] == '|')
		(*i)++;
	else if (s[*i] == '"' || s[*i] == '\'')
		count_quotes(s, i);
	else if (s[*i] == '$')
		count_dollar(s, i);
	else if (ft_isredirection(s[*i]))
	{
		while (ft_isredirection(s[*i]))
			(*i)++;
	}
	else
	{
		while (!ft_isspace(s[*i]) && s[*i] != '|' && s[*i] && !ft_isdelimiter(s[*i]) && !ft_isredirection(s[*i]))
			(*i)++;
	}
}

int	count_args(const char *s)
{
	int	i;
	int	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] == '\0')
			break;
		word++;
		if (ft_isspace(s[i]) && (word == 1 || s[i-1] == '|' )) // Trocar para identificar após primeiro argumento
			skip_spaces(s, &i);
		count_arg(s, &i);
	}
	return (word);
}

