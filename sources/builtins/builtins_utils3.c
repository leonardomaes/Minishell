/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 01:25:10 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/02/19 23:44:52 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//check if string contains a valid number
int	ft_isnumber(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

//export: helper funtion to count the number of env variables
int	env_var_count(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp && envp[i])
		i++;
	return (i);
}

//export: helper function to identify appending operations
int	is_append_operation(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (1);
		if (str[i] == '=')
			break ;
	}
	return (0);
}

//export: helper function to extract the name of var from a str
char	*get_var_name(const char *str)
{
	int		i;
	char	*name;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '+' && str[i] != '=')
		i++;
	name = ft_substr(str, 0, i);
	return (name);
}

//export: helper function to extract the value to append to a var
char	*get_append_value(const char *str)
{
	while (*str && *str != '=')
		str++;
	if (*str == '=')
		str++;
	return (ft_strdup(str));
}
