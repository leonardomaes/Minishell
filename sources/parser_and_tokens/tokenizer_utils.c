/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:20:10 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 23:31:15 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	double_quote_lenght(t_msh *msh, const char *s, int *i)
{
	int	j;

	j = 0;
	if (s[*i] == '"')
	{
		(*i)++;
		j++;
	}
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '$' && !ft_isdelimiter(s[*i + 1]) && !ft_isspace(s[*i
					+ 1]))
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

int	single_quote_lenght(const char *s, int *i)
{
	int	j;

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

int	environ_lenght(t_msh *msh, const char *s, int *i)
{
	char	*env;
	char	*env_val;
	int		j;

	if (s[*i] == '$')
		(*i)++;
	j = *i;
	if (s[j] == '?')
		return ((*i)++, j = 3, j);
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

int	handle_environ(t_msh *msh, const char *s, char *str, int *l)
{
	const char	*start;
	char		var_name[999];
	char		*env_value;
	int			len;

	len = 0;
	start = &s[++(*l)];
	if (*start == '?')
	{
		env_value = ft_itoa(g_exit);
		len = ft_strlen(env_value);
		ft_strcpy(str, env_value);
		(*l)++;
		return (free(env_value), len);
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
	return (str[len] = '\0', len);
}

int	handle_single_quote(const char *s, char *str, int *l)
{
	int	len;

	len = 0;
	str[len++] = s[(*l)++];
	while (s[*l] && s[*l] != '\'')
		str[len++] = s[(*l)++];
	if (s[*l] == '\'')
		str[len++] = s[(*l)++];
	str[len] = '\0';
	return (len);
}
