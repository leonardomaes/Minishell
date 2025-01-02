/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/01/02 01:27:42 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//-----INFO FOR TESTING PORPUSES-----
// do not forget to compile with libft (make on libft first): gcc -Wall -Wextra -Werror -o test_unset unset.c -L../../includes/libft -lft

//including the libraries needed for testing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/libft/libft.h"

//creating a t_msh structure that includes the env var array
typedef struct s_msh
{
	char **envp;
}	t_msh;

void	free_ptr(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
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

//helper funtion to count the number of env variables
int	env_var_count(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
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
			free_ptr(msh->envp[i]);
			i++;
		}
	}
	free(msh->envp);
	return (new_envp);
}

//-----END OF INFO FOR TESTING PORPUSES-----

//helper function to remove an env var from the list
int	remove_env_var(t_msh *msh, char *var)
{
	int		i;
	int		j;
	size_t	len;
	int		env_count;
	char	**new_envp;
	
	i = 0;
	j = 0;
	len = ft_strlen(var);
	//count the variavles and allocate memory for the new array
	env_count = env_var_count(msh->envp);
	new_envp = ft_calloc(env_count + 1, sizeof(char *));
	if (!new_envp)
		return (1);
	//copy variables for new list excluding the ones to remove
	while (msh->envp[i])
	{
		if (strncmp(msh->envp[i], var, len) == 0 
		&& msh->envp[i][len] == '=' || msh->envp[i][len] == '\0')
		{
			free(msh->envp[i]);
			i++;
			continue ;
		}
		new_envp[j++] = msh->envp[i];
		i++;
	}
	new_envp[j] = NULL;
	//free old array and change pointer to the new array
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

//-----FUNCTIONS FOR TESTING PORPUSES-----

//function to print env vars list
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

//value attribution to my false envp array, define arguments
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

	// Test 1: remove an existing variable
	printf("\nTest 1: unset PATH\n");
	args[0] = "unset";
	args[1] = "PATH";
	args[2] = NULL;
	execute_unset(&msh, args);
	print_envp(msh.envp);

	// Test 2: remove a non existing variable
	printf("\nTest 2: unset NON_EXISTENT\n");
	args[1] = "NON_EXISTANTE";
	execute_unset(&msh, args);
	print_envp(msh.envp);

	// Test 3: use an invalid argument
	printf("\nTest 3: unset 1VAR\n");
	args[1] = "1VAR";
	execute_unset(&msh, args);
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
