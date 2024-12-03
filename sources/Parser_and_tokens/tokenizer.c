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

void	free_array(char **str, unsigned int n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	count_args(const char *s)
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
		else
		{
			while (!ft_isspace(s[i]) && s[i])
				i++;
		}
	}
	return (word);
}

int	*calculate_lengths(const char *s, int words)
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
        if (s[i] == '"')
        {
            i++;
            while (s[i] && s[i] != '"')
            {
                len[word]++;
                i++;
            }
            if (s[i] == '"')
                i++;
        }
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
    return len;
}

char	**ft_split_args(const char *s)
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
	str = (char **)malloc(sizeof(char *) * (words + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < words)				// Trocar por uma func para alocar 'str'
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
	i = 0;
	while (*s)
	{
		j = 0;
		while (*s && ft_isspace(*s))
			s++;
		if (*s == '\0')
			break ;
		if (*s == '"')				// Trocar por um handle double quotes e single quotes
		{
			s++;
			while (*s != '"')
				str[i][j++] = *s++;
			if (*s == '"')
				s++;
		}
		else
		{
			while (*s && !ft_isspace(*s))
				str[i][j++] = *s++;
		}
		str[i][j] = '\0';
		i++;
	}
	str[i] = NULL;
	free(len);
	return (str);
}

void	split_tokens(t_msh *msh, t_tokens **token, t_tokens *prev, int i)
{
	t_tokens	*temp;
	
	if (i < msh->data->argc)
	{
		*token = NULL;
		*token = malloc(sizeof(t_tokens));
		temp = *token;
		temp->count = i;
		temp->name = msh->data->args[i];
		temp->type = get_type(msh->data->args[i]);
		i++;
		temp->next = NULL;
		if (prev != NULL)
			temp->prev = prev;
		split_tokens(msh, &temp->next, temp, i);
	}
}
