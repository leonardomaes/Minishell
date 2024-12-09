/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/25 19:03:46 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *expand_env(char **envp, char *name)		// Expandir de acordo com variavel apos '$'
{
	if (!*envp)
    	return (NULL);
	while (ft_strncmp(name, *envp, 4))
		envp++;
	return (*envp + ft_strlen(name));
}

char	**ft_get_env(char **envp)
{
	char	**env_copy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env_copy = malloc(sizeof(char *) * (i + 1));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env_copy[i] = ft_strdup(envp[i]);
		if (!env_copy)
			return (NULL);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

char	*ft_get_path(char **envp)
{
	if (!*envp)
    	return (NULL);
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	return (*envp + 5);
}

char	*ft_get_command(char *cmd, char **path)
{
	char	*comm;
	char	*temp;

	while (*path)
	{
		temp = ft_strjoin(*path, "/");
		if (!temp)
		{
			perror("Error allocating memory for command");
			return (NULL);
		}
		comm = ft_strjoin(temp, cmd);
		free(temp);
		if (!comm)
		{
			perror("Error allocating memory for command");
			return (NULL);
		}
		if (access(comm, 0) == 0)
			return (comm);
		free(comm);
		path++;
	}
	return (NULL);
}
