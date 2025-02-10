/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/10 19:41:59 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//used a simple buble sort to sort envp array
void	ft_sort_array(char **array, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(array[j], array[j + 1], ft_strlen(array[j])) > 0)
			{
				tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

//function to print an array (added "declare..." to be used with export)
void	ft_print_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		printf("declare -x %s\n", array[i]);
		i++;
	}
}

//function to free an array (used whith envp)
void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free_ptr((void **)&array[i]);
		i++;
	}
	free(array);
}

//check if env variable name is valid
int	is_valid_var_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0])
		return (0);
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (0); 
	while (name[i] && name[i] != '=')
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
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
		sorted_envp[i] = ft_strdup(envp[i]);
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

int	execute_export(t_msh *msh, char **args)
{
	int		i;
	char	*name;
	char	*value;
	int		status;

	i = 1;
	status = 0;
	if (!args[1])
	{
		print_sorted_env(msh->envp);
		return (0);
	}
	while(args[i])
	{
		if (ft_strcmp(args[i], "_") == 0) //special case for '_'
		{
			i++;
			continue ;
		}
		name = ft_strdup(args[i]);
		value = ft_strchr(name, '=');
		if (value)
		{
			*value = '\0'; //if '=' exists, replace it with a '\0', splitting the string in two parts (name and value). Value kept as NULL if there's no '='
			value++;
		}
		if (!is_valid_var_name(name))
		{
			ft_putstr_fd("bash: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
			free_ptr((void **)&name);
			i++;
			continue ;
		}
		if (set_env_var(msh, name, value))
		{
			free_ptr((void **)&name);
			return (1); //funtion set_env_var returned error
		}
		free_ptr((void **)&name);
		i++;
	}
	g_exit = status;
	return (status);
}
