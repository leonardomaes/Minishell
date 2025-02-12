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

char **alloc_args(int words, int *len)
{
	char **str;
	int i;

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


