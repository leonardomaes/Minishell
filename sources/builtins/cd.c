/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/10 18:22:35 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//helper funtion to update env variables when changing dir
int	update_env_change_dir(char *oldpwd, t_msh *msh)
{
	char	*cwd;

	if (set_env_var(msh, "OLDPWD", oldpwd) != 0)
	{
		free_ptr((void **)&oldpwd);
		return (1);
	}
	free_ptr((void **)&oldpwd);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("cd: getcwd failed");
		return (1);
	}
	if (set_env_var(msh, "PWD", cwd) != 0)
	{
		free_ptr((void **)&cwd);
		return (1);
	}
	free_ptr((void **)&cwd);
	return (0);
}

//helper function to change directories
int	change_dir(const char *path, t_msh *msh)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(path) != 0)
	{
		perror("cd");
		free_ptr((void **)&oldpwd);
		return (1);
	}
	if (update_env_change_dir(oldpwd, msh) != 0)
		return (1);
	return (0);
}

//main funtion do deal with cd builtin
int	execute_cd(t_msh *msh, char **args)
{
	char	*path;

	if (!args[1] || args[1][0] == '\0' || ft_isspace(args[1][0])
		|| ft_strncmp(args[1], "--", 3) == 0)
	{
		path = get_env_var_value(msh->envp, "HOME");
		if (!path || !*path)
			return (ft_putstr_fd("bash: cd: HOME not set\n", 2), 1);
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_var_value(msh->envp, "OLDPWD");
		if (!path)
			return (ft_putstr_fd("bash: cd: OLDPWD not set\n", 2), 1);
	}
	else if (args[2])
		return (ft_putstr_fd("bash: cd: too many arguments\n", 2), 1);
	else
		path = args[1];
	return (change_dir(path, msh));
}
