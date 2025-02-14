/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 23:57:54 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/02/14 00:47:47 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"    

//reallocate memory for a new env list and returns a pointer to it
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
			if (!new_envp[i])
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

//helper function to get the index of a env variable
int	get_env_var_index(char **envp, char *var_name)
{
	int				i;
	size_t			var_name_len;

	i = 0;
	var_name_len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_name_len) == 0
			&& (envp[i][var_name_len] == '='
			|| envp[i][var_name_len] == '\0'))
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
			free_ptr((void **)&tmp);
			return (ft_strchr(envp[i], '=') + 1);
		}
		i++;
	}
	free_ptr((void **)&tmp);
	return (NULL);
}

//helper function that creates a new variable entry
static int	create_env_entry(char *var_name, char *var_value, char **new_entry)
{
	char	*temp;

	if (!var_name || !new_entry)
		return (1);
	if (!var_value)
	{
		*new_entry = ft_strdup(var_name);
		if (!*new_entry)
			return (1);
		return (0);
	}
	*new_entry = ft_strjoin(var_name, "=");
	if (!*new_entry)
		return (1);
	temp = ft_strjoin(*new_entry, var_value);
	free(*new_entry);
	if (!temp)
		return (1);
	*new_entry = temp;
	return (0);
}

//function to update a variable if it exists or create it if not
int	set_env_var(t_msh *msh, char *var_name, char *var_value)
{
	int		idx;
	char	*new_entry;

	if (create_env_entry(var_name, var_value, &new_entry))
		return (1);
	idx = get_env_var_index(msh->envp, var_name);
	if (idx != -1 && msh->envp[idx])
	{
		free_ptr((void **)&msh->envp[idx]);
		msh->envp[idx] = new_entry;
		return (0);
	}
	idx = env_var_count(msh->envp);
	msh->envp = realloc_env_vars(msh, idx + 2);
	if (!msh->envp)
	{
		free(new_entry);
		return (1);
	}
	msh->envp[idx] = new_entry;
	msh->envp[idx + 1] = NULL;
	return (0);
}
