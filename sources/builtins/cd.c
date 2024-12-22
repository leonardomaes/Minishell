/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/22 16:30:47 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

//helper function to change directories
int	change_dir(char *path, char **envp)
{

}

//main funtion do deal with cd builtin
int	execute_cd(t_msh *msh, char **args) //later we can improve how we deal with error messages in a centralized way and reduce the size of the function
{
	char	*path;

	if (!args || !args[1] || ft_isspace(args[1][0] || args[1][0] == '\0' || ft_strncmp(args[1], "--", 3) == 0))
	{
		path = get_env_var_value(msh->envp, "HOME");
		if (!path || !*path || ft_isspace(*path))
		{
			printf("-bash: cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_var_value(msh->envp, "OLDPWD");
		if (!path)
		{
			printf("-bash: cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", path);
	}
	else if (args[2])
	{
		printf("-bash: cd: too many arguments\n");
		return (1);
	}
	else
		path = args[1];
	return (change_dir(path, msh->envp));
}
