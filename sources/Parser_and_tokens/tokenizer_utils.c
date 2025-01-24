/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:20:10 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/15 20:20:15 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int double_quote_lenght(const char *s, int *i) //Calculate size of strings in double quotes, including env vars
{
	int		j;

	j = 0;
	if (s[*i] == '"')
	{
		(*i)++;
		j++;
	}
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '$')
			j += environ_lenght(s, i);
		else
		{
			j++;
			(*i)++;
		}
	}
	if (s[*i] == '"')
	{
		(*i)++;
		j++;
	}
	return (j);
}

int single_quote_lenght(const char *s, int *i) //Calculate size of strings in double quotes
{
	int		j;

	j = 0;
	if (s[*i] == '\'')
	{
		(*i)++;
		j++;
	}
	while (s[*i] && s[*i] != '\'')
	{
		j++;
		(*i)++;
	}
	if (s[*i] == '\'')
	{
		(*i)++;
		j++;
	}
	return (j);
}

int	environ_lenght(const char *s, int *i) //Calculate size of extended env vars
{
	char *env;
	char *env_val;
	int		j;
	
	if (s[*i] == '$')
		(*i)++;
	j = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	env = (char *)malloc(sizeof(char) * ((*i - j) + 1));
	if (!env)
		return (0);
	ft_strlcpy(env, &s[j], (*i - j) + 1);
	env_val = getenv(env);
	if (!env_val)
		j = ft_strlen(env);
	else
		j = ft_strlen(env_val);
	free(env);
	return (j);
}

int	handle_environ(const char **s, char *str)	// Expand env vars
{
	const char *start;
	char *env_value;
	char var_name[9999];
	int len;

	len = 0;
	(*s)++;
	if (**s == '?')
	{
		str = ft_itoa(g_exit);
		return (1);
	}
	start = *s;
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	ft_strlcpy(var_name, start, ((*s - start) + 1));
	env_value = getenv(var_name);
	if (env_value)
	{
		while (*env_value)
		{
			*str++ = *env_value++;
			len++;
		}
	}
	*str = '\0';
	return (len);
}

// Do not must to interpret metachars
int handle_single_quote(const char **s, char *str)	// Identifies quotes and treat it
{
	int i;

	i = 0;
	str[i++] = **s;
	(*s)++;
	while (**s && **s != '\'')
	{
		str[i++] = **s;
		(*s)++;
	}
	if (**s == '\'')
	{
		str[i++] = **s;
		(*s)++;
	}
	str[i] = '\0';
	return (ft_strlen(str));
}

int	handle_double_quote(const char **s, char *str) // Identifies quotes and '$'
{
	int len;
	int i;

	len = 0;
	*str = **s;
	str++;
	(*s)++;
	while (**s && **s != '"')
	{
		if (**s == '$')
		{
			i = handle_environ(s, str);
			str += i;
			len += i;
		}
		else
		{
			*str = **s;
			str++;
			(*s)++;
			len++;
		}
	}
	if (**s == '"')
	{
		*str = **s;
		(*s)++;
	}
	str++;
	*str = '\0';
	return (len);
}

char *ft_chartrim(char **s, char set)
{
    char *start;
    char *end;
    char *new_str;

    if (!s || !*s) // Verifica se a string é válida
        return NULL;

    start = *s;
    end = *s + strlen(*s) - 1;
    while (*start == set)
        start++;
    while (end > start && *end == set)
        end--;
    new_str = strndup(start, end - start + 1);
	return (new_str);
}
