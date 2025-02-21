/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 02:38:59 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_env(char **envp, char *name)
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
	{
		envp++;
		if (*envp == NULL)
			return (NULL);
	}
	return (*envp + 5);
}

char	*ft_find_executable(char *cmd, char **path)
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
		if (access(comm, F_OK) == 0)
			return (comm);
		free(comm);
		path++;
	}
	return (NULL);
}

char	*ft_get_command(t_msh *msh, char *cmd, char **path)
{
	char	*comm;

	if (!cmd)
	{
		ft_free_all(msh);
		exit(1);
	}
	else if (cmd[0] == '\0')
		return (NULL);
	if (!path)
		return (ft_strdup(cmd));
	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
	}
	else
	{
		comm = ft_get_bcmd(cmd);
		if (access(comm, F_OK) == 0)
			return (comm);
		free(comm);
	}
	comm = ft_find_executable(cmd, path);
	if (cmd[0] == '/')
		ft_exit(msh, 127, ": No such file or directory\n", ft_strdup(cmd));
	return (comm);
}
