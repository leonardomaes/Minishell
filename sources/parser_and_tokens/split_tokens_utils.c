/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:19:39 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/28 18:19:39 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**alloc_args(int words, int *len)
{
	char	**str;
	int		i;

	str = (char **)malloc(sizeof(char *) * (words + 1));
	if (!str)
		return (NULL);
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

char	**ft_split_args(t_msh *msh, const char *s)
{
	char	**str;
	int		*len;
	t_split	nums;

	if (!s)
		return (NULL);
	nums.words = count_args(s);
	len = calculate_lengths(msh, s, nums.words);
	if (!len)
		return (NULL);
	str = alloc_args(nums.words, len);
	if (!str)
	{
		free(len);
		return (NULL);
	}
	nums.i = 0;
	nums.l = 0;
	while (s[nums.l])
		split_arg(msh, &str, s, &nums);
	str[nums.i] = NULL;
	free(len);
	return (str);
}
