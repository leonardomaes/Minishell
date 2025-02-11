/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/11 19:02:35 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//helper funtion to count the number of env variables
int	env_var_count(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

//helper function that formats each line of envp to print
static char	*format_env_var(char *env_var)
{
	char	*eq_pos;
	char	*name;
	char	*value;
	char	*formatted;

	eq_pos = ft_strchr(env_var, '=');
	if (!eq_pos)
		return (ft_strdup(env_var));
	name = ft_substr(env_var, 0, eq_pos - env_var);
	value = eq_pos + 1;
	formatted = ft_strjoin4(name, "=\"", value, "\"");
	free(name);
	return (formatted);
}

//sorted print of the env variable 
int	print_sorted_env(char **envp)
{
	int		count;
	char	**sorted_envp;
	int		i;

	count = env_var_count(envp);
	sorted_envp = ft_calloc(count + 1, sizeof(*sorted_envp));
	if (!sorted_envp)
		return (1);
	i = 0;
	while (i < count)
	{
		sorted_envp[i] = format_env_var(envp[i]);
		if (!sorted_envp[i])
		{
			ft_free_array(sorted_envp);
			return (1);
		}
		i++;
	}
	ft_sort_array(sorted_envp, count);
	ft_print_array(sorted_envp);
	ft_free_array(sorted_envp);
	return (0);
}

static int	process_export_arg(t_msh *msh, char *arg, int *status)
{
	char	*name;
	char	*value;

	if (ft_strcmp(arg, "_") == 0)
		return (0);
	name = ft_strdup(arg);
	value = ft_strchr(name, '=');
	if (value)
	{
		*value = '\0';
		value++;
	}
	if (!is_valid_var_name(name))
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		*status = 1;
		free_ptr((void **)&name);
		return (0);
	}
	if (set_env_var(msh, name, value))
		return ((free_ptr((void **)&name), 1));
	free_ptr((void **)&name);
	return (0);
}

//main function do deal with export builtin
int	execute_export(t_msh *msh, char **args)
{
	int		i;
	int		status;

	i = 1;
	status = 0;
	if (!args[1])
	{
		print_sorted_env(msh->envp);
		return (0);
	}
	while (args[i])
	{
		if (process_export_arg(msh, args[i], &status))
			return (1);
		i++;
	}
	g_exit = status;
	return (status);
}
