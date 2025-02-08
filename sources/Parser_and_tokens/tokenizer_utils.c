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

int double_quote_lenght(t_msh *msh, const char *s, int *i) //Calculate size of strings in double quotes, including env vars
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
		if (s[*i] == '$' && !ft_isdelimiter(s[*i+1]) && !ft_isspace(s[*i+1])) // Dolar
			j += environ_lenght(msh, s, i);
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

int	environ_lenght(t_msh *msh, const char *s, int *i) //Calculate size of extended env vars
{
	char *env;
	char *env_val;
	int		j;
	
	if (s[*i] == '$')
		(*i)++;
	j = *i;
	if (s[j] == '?')
		return (j=3, j);
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	env = (char *)malloc(sizeof(char) * ((*i - j) + 1));
	if (!env)
		return (0);
	ft_strlcpy(env, &s[j], (*i - j) + 1);
	env_val = get_env_var_value(msh->envp, env);
	if (!env_val)
		j = ft_strlen(env);
	else
		j = ft_strlen(env_val);
	free(env);
	return (j);
}

int handle_environ(t_msh *msh, const char *s, char *str, int *l)	// Expand env vars
{
	const char *start = &s[++(*l)];
	char var_name[999];
	char *env_value;
	int len = 0;

	if (*start == '?')
	{
		env_value = ft_itoa(g_exit);
		len = ft_strlen(env_value);
		ft_strlcpy(str, env_value, len + 1);
		free(env_value);
		(*l)++;	// Avançar '?'
		return (len);
	}
	while (s[*l] && (ft_isalnum(s[*l]) || s[*l] == '_'))
		(*l)++;
	ft_strlcpy(var_name, start, &s[*l] - start + 1);
	env_value = get_env_var_value(msh->envp, var_name);
	if (env_value)
	{
		while (*env_value)
			str[len++] = *env_value++;
	}
	str[len] = '\0';
	return (len);
}

int handle_single_quote(const char *s, char *str, int *l) // Identifies quotes and treat it
{
	int len = 0;

	str[len++] = s[(*l)++];
	while (s[*l] && s[*l] != '\'')
		str[len++] = s[(*l)++];
	if (s[*l] == '\'')
		str[len++] = s[(*l)++];
	str[len] = '\0';
	return (len);
}

int handle_double_quote(t_msh *msh, const char *s, char *str, int *l) // Identifies quotes and '$'
{
	int len = 0;
	int i;

	str[len++] = s[(*l)++];
	while (s[*l] && s[*l] != '"')
	{
		if (s[*l] == '$' && !ft_isdelimiter(s[*l+1]) && !ft_isspace(s[*l+1])) // Dolar
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
