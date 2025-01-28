/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/01/28 00:18:52 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
//-----INFO FOR TESTING PORPUSES-----
// do not forget to compile with libft (make on libft first): gcc -Wall -Wextra -Werror -o test_export export.c -L../../includes/libft -lft

//including the libraries needed for testing
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../includes/libft/libft.h"

//creating a t_msh structure that includes the env var array
typedef struct s_msh
{
	char **envp;
} t_msh;

void	free_ptr(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
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
					free(new_envp[i]);
				free(new_envp);
				return (NULL);
			}
			free_ptr((void **)(&msh->envp[i]));
			i++;
		}
	}
	free(msh->envp);
	return (new_envp);
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
		free_ptr((void **)&msh->envp[idx]);
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
	free_ptr((void*)&tmp);
	return (0);
}

//-----END OF INFO FOR TESTING PORPUSES-----
*/
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

int	is_valid_var_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	while (name[i] && name[i] != '=')
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
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
	int		status; 

	i = 1;
	status = 0; 
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
/*
//-----FUNCTIONS FOR TESTING PORPUSES-----

//function to test env vars list
void	print_envp(char **envp)
{
	int	i;

	i = 0;
	printf("\n---ENV VARS---\n");
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	printf("------------------\n");
}

//value attribution to my false envp array, define cd arguments
int	main(void)
{
	//creating a struct, an array of env vars, an array or cd arguments, and a counter var (i)
	t_msh	msh;
	char	*test_envp[] = {
			"PATH=/user/bin",
			"HOME=/home/test_user",
			"SHELL=/bin/bash",
			NULL	
			};
	char	*args[3];
	int		i = 0;

    // Initialize msh.envp
    msh.envp = NULL;
    msh.envp = realloc_env_vars(&msh, 3);
    while (test_envp[i])
    {
        msh.envp[i] = ft_strdup(test_envp[i]);
        i++;
    }

	//print inicial envp array to check if everything is ok
	printf("---Initial Env Variables---\n");
	print_envp(msh.envp);

	// Test 1: export a new variable
	printf("\nTest 1: export TEMP_VAR=42\n");
	args[0] = "export";
	args[1] = "TEMP_VAR=42";
	args[2] = NULL;
	execute_export(&msh, args);
	print_envp(msh.envp);

	// Test 2: update an existing variable
	printf("\nTest 2: export PATH=/usr/local/bin\n");
	args[1] = "PATH=/usr/local/bin";
	execute_export(&msh, args);
	print_envp(msh.envp);

	// Test 3: use an invalid argument
	printf("\nTest 3: export 1VAR=value\n");
	args[1] = "1VAR=value";
	execute_export(&msh, args);
	print_envp(msh.envp);

	// Test 4: export without arguments
	printf("\nTest 4: export\n");
	args[1] = NULL;
	execute_export(&msh, args);
	print_envp(msh.envp);

	//cleanup function
	i = 0;
	while (msh.envp[i])
	{
		free_ptr((void**)&msh.envp[i]);
		i++;
	}
	free_ptr((void **)&msh.envp);
	return (0);
}
*/
