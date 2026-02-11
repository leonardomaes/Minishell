/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:20:10 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 23:29:14 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*calculate_lengths(t_msh *msh, const char *s, int words)
{
	int		*len;
	t_split	nums;

	nums.i = 0;
	nums.words = 0;
	len = (int *)malloc(sizeof(int) * words);
	if (!len)
		return (NULL);
	while (s[nums.i] && nums.words < words)
	{
		len[nums.words] = 0;
		if (s[nums.i] == '\0')
			break ;
		if (ft_isspace(s[nums.i]) && (nums.words == 0 || s[nums.i - 1] == '|'))
			skip_spaces(s, &nums.i);
		calculate_length(msh, s, &nums, &len);
		nums.words++;
	}
	return (len);
}

int	handle_double_quote(t_msh *msh, const char *s, char *str, int *l)
{
	int	len;
	int	i;

	len = 0;
	str[len++] = s[(*l)++];
	while (s[*l] && s[*l] != '"')
	{
		if (s[*l] == '$' && !ft_isdelimiter(s[*l + 1])
			&& !ft_isspace(s[*l + 1]))
		{
			i = handle_environ(msh, s, &str[len], l);
			len += i;
		}
		else
			str[len++] = s[(*l)++];
	}
	if (s[*l] == '"')
		str[len++] = s[(*l)++];
	str[len] = '\0';
	return (len);
}

char	*ft_chartrim(char **s, char set)
{
	char	*start;
	char	*end;
	char	*new_str;

	if (!s || !*s)
		return (NULL);
	start = *s;
	end = *s + ft_strlen(*s) - 1;
	while (*start == set)
		start++;
	while (end > start && *end == set)
		end--;
	new_str = ft_strndup(start, end - start + 1);
	return (new_str);
}
