/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/30 01:50:48 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//used a simple buble sort (evaluate other more robust algorithms if necessary)
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

//function to print an array (envp - changed to be used with export)
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

int	is_valid_van_name(char *name)
{
	int i;

	i = 0;
	if (!name | !(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	while (name[i] && name[i] != '=')
	{
		if (!(ft_isalpha(name[0]) || name[0] == '_'))
			return (0);
	}
	return (1);
}


int	print_sorted_env(char **envp)
{
	int		count;
	char	**sorted_envp;
	int		i;

	count = env_var_count(envp);
	sorted_envp = ft_calloc(count + 1, sizeof(*sorted_envp));
	if (!sorted_envp)
		return (1);
	//copy env variables into sorted_env
	i = 0;
	while (i < count)
	{
		sorted_envp[i] = ft_strdup(envp[i]);
		if (!sorted_envp[i]) //if ft_strdup fails
		{
			ft_free_array(sorted_envp);
			return (1); 
		}
		i++;
	}
	//run helper functions to sort, print and free the array
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

	i = 1;
	//if no arguments display env vars sorted
	if (!args[1])
	{
		print_sorted_env(msh->envp);
		return (0);
	}
	while(args[i])
	{
		name = ft_strdup(args[i]);
		value = ft_strchr(name, '=');
		if (value)
		{
			*value = '\0'; //if '=' exists, replace it with a '\0', splitting the string in two parts (name and value). Value kept as NULL if there's no '='
			value++;
		}
		if (!is_valid_var_name(name))
		{
			printf("bash: export: `%s`: not a valid identifier\n", args[i]);
			free_ptr(&name);
			i++;
			continue ;
		}
		if (set_env_var(msh, name, value))
		{
			free_ptr(&name);
			return (1); //funtion set_env_var returned error
		}
		free_ptr(&name);
		i++;
	}
	return (0);
}

