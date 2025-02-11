/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/11 19:32:57 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	copy_env_vars(char **new_envp, char **old_envp, char *var, int *j)
{
	int		i;
	size_t	len;

	if (!new_envp || !old_envp || !var || !j)
		return (1);
	i = 0;
	len = ft_strlen(var);
	while (old_envp[i])
	{
		if (!(strncmp(old_envp[i], var, len) == 0
				&& (old_envp[i][len] == '=' || old_envp[i][len] == '\0')))
		{
			new_envp[*j] = ft_strdup(old_envp[i]);
			if (!new_envp[*j])
			{
				ft_free_array(new_envp);
				return (1);
			}
			(*j)++;
		}
		i++;
	}
	return (0);
}

int	remove_env_var(t_msh *msh, char *var)
{
	int		j;
	int		env_count;
	char	**new_envp;

	if (!msh || !var)
		return (1);
	j = 0;
	env_count = env_var_count(msh->envp);
	new_envp = ft_calloc(env_count + 1, sizeof(char *));
	if (!new_envp)
		return (1);
	if (copy_env_vars(new_envp, msh->envp, var, &j))
		return (1);
	new_envp[j] = NULL;
	ft_free_array(msh->envp);
	msh->envp = new_envp;
	return (0);
}

int	execute_unset(t_msh *msh, char **args)
{
	int	i;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (is_valid_var_name(args[i]))
			remove_env_var(msh, args[i]);
		i++;
	}
	return (0);
}
