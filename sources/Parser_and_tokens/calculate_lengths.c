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

void	calculate_quotes(t_msh *msh, const char *s, t_split *nums, int **len)
{
	if (s[nums->i] == '\'')
		(*len)[nums->words] = single_quote_lenght(s, &nums->i);
	else if (s[nums->i] == '"')
		(*len)[nums->words] = double_quote_lenght(msh, s, &nums->i);
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

void	calculate_else(const char *s, t_split *nums, int **len)
{
	while (s[nums->i] && s[nums->i] != '|' && !ft_isspace(s[nums->i])
		&& !ft_isdelimiter(s[nums->i]) && !ft_isredirection(s[nums->i]))
	{
		(*len)[nums->words]++;
		(nums->i)++;
	}
}

void	calculate_length(t_msh *msh, const char *s, t_split *nums, int **len)
{
	if (ft_isspace(s[nums->i]))
	{
		(*len)[nums->words] = 1;
		skip_spaces(s, &nums->i);
	}
	else if (s[nums->i] == '|')
		sum_one(nums->words, &nums->i, len);
	else if (s[nums->i] == '$' && !ft_isdelimiter(s[nums->i + 1])
		&& !ft_isspace(s[nums->i + 1]))
		(*len)[nums->words] = environ_lenght(msh, s, &nums->i);
	else if (s[nums->i] == '$' && (s[nums->i + 1] == '\0'
			|| s[nums->i + 1] == ' '))
		sum_one(nums->words, &nums->i, len);
	else if (s[nums->i] == '\'' || s[nums->i] == '"')
		calculate_quotes(msh, s, nums, len);
	else if (ft_isredirection(s[nums->i]))
		calculate_redir(s, nums->words, &nums->i, len);
	else
		calculate_else(s, nums, len);
}
