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

#include "../../includes/minishell.h"

void	add_one(char ***str, const char *s, int *j, t_split *nums)
{
	(*str)[nums->i][(*j)++] = s[(nums->l)++];
	(*str)[nums->i][*j] = '\0';
}

void	split_redirs(char ***str, const char *s, int *j, t_split *nums)
{
	(*str)[nums->i][(*j)++] = s[(nums->l)++];
	if (ft_isredirection(s[nums->l]))
		(*str)[nums->i][(*j)++] = s[(nums->l)++];
	(*str)[nums->i][*j] = '\0';
}

void	split_else(char ***str, const char *s, int *j, t_split *nums)
{
	while (s[nums->l] && s[nums->l] != '|' && !ft_isspace(s[nums->l])
		&& !ft_isdelimiter(s[nums->l]) && !ft_isredirection(s[nums->l]))
		(*str)[nums->i][(*j)++] = s[(nums->l)++];
	(*str)[nums->i][*j] = '\0';
}

int	split_quotes(t_msh *msh, const char *s, char ***str, t_split *nums)
{
	if (s[nums->l] == '"')
		return (handle_double_quote(msh, s, (*str)[nums->i], &nums->l));
	else if (s[nums->l] == '\'')
		return (handle_single_quote(s, (*str)[nums->i], &nums->l));
	return (0);
}

void	split_arg(t_msh *msh, char ***str, const char *s, t_split *nums)
{
	int	j;

	j = 0;
	if (ft_isspace(s[nums->l]) && (nums->i == 0 || s[nums->l - 1] == '|'))
		skip_spaces(s, &nums->l);
	if (ft_isspace(s[nums->l]))
	{
		add_one(str, s, &j, nums);
		skip_spaces(s, &nums->l);
	}
	else if (s[nums->l] == '|')
		add_one(str, s, &j, nums);
	else if (s[nums->l] == '"' || s[nums->l] == '\'')
		j = split_quotes(msh, s, str, nums);
	else if (s[nums->l] == '$' && !ft_isdelimiter(s[nums->l + 1])
		&& !ft_isspace(s[nums->l + 1]))
		j = handle_environ(msh, s, (*str)[nums->i], &nums->l);
	else if (s[nums->l] == '$' && (s[nums->l + 1] == '\0'
			|| ft_isspace(s[nums->l + 1]) || ft_isdelimiter(s[nums->l + 1])))
		add_one(str, s, &j, nums);
	else if (s[nums->l] && ft_isredirection(s[nums->l]))
		split_redirs(str, s, &j, nums);
	else
		split_else(str, s, &j, nums);
	nums->i++;
}
