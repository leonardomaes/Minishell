/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 23:57:54 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/01/02 15:53:22 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"    

//helper function to reallocate memory for a new env variable / returns a
//pointer to the new env variable list
char	**realloc_env_vars(t_msh *msh, int size)
{
	char	**new_envp;
	int		i;

	new_envp = ft_calloc(size + 1, sizeof * new_envp);
	if (!new_envp)
		return (NULL);
	i = 0;
	if (msh->envp)
	{
		while (msh->envp[i] && i < size)
		{
			new_envp[i] = ft_strdup(msh->envp[i]);
			if (!new_envp[i]) //additional safeguard - check if neeed | if memory allocation for new_envp[i] fails it cleans all previously allocated memory 
			{
				while (--i >= 0)
					free_ptr((void **)&msh->envp[i]);
				free(new_envp);
				return (NULL);
			}
			free_ptr((void **)&msh->envp[i]);
			i++;
		}
	}
	free(msh->envp);
	return (new_envp);
}

//helper funtion to count the number of env variables
int	env_var_count(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

//helper function to get the index of a env variable
int	get_env_var_index(char **envp, char *var_name)
{
	int		i;
	size_t	var_name_len;

	i = 0;
	var_name_len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_name_len) == 0
			&& envp[i][var_name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

//helper function to get a var variable from env list
char	*get_env_var_value(char **envp, char *var)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(tmp, envp[i], ft_strlen(tmp)) == 0)
		{
			free_ptr(tmp);
			return (ft_strchr(envp[i], '=') + 1);
		}
		i++;
	}
	free_ptr(tmp);
	return (NULL);
}

//helper function to update a variable if it exists or create it if not
int	set_env_var(t_msh *msh, char *var_name, char *var_value)
{
	int		idx;
	char	*tmp;

	idx = get_env_var_index(msh->envp, var_name);
	if (var_value == NULL)
		var_value = "";
	tmp = ft_strjoin("=", var_value);
	if (!tmp)
		return (1);
	if (idx != -1 && msh->envp[idx])
	{
		free_ptr(&msh->envp[idx]);
		msh->envp[idx] = ft_strjoin(var_name, tmp);
	}
	else
	{
		idx = env_var_count(msh->envp);
		msh->envp = realloc_env_vars(msh, idx + 1);
		if (!msh->envp)
			return (1);
		msh->envp[idx] = ft_strjoin(var_name, tmp);
	}
	free_ptr(&tmp);
	return (0);
}
