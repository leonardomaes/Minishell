/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/28 23:55:05 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
//-----INFO FOR TESTING PORPUSES-----
// do not forget to compile with libft: cc -Wall -Wextra -Werror -o test_cd cd_builtin.c -L../../includes/libft -lft

//creating a t_msh structure that includes the env var array
typedef struct s_msh
{
	char **envp;
} t_msh;

//including the libraries needed for testing
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../includes/libft/libft.h"

void	free_ptr(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

//-----END OF INFO FOR TESTING PORPUSES-----
*/
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
		free_ptr(msh->envp[idx]);
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
	free_ptr(tmp);
	return (0);
}
//helper funtion to update env variables when changing dir
int	update_env_change_dir(char *oldpwd, t_msh *msh)
{
	char	*cwd;
	//update OLDPWD env variable
	if (set_env_var(msh, "OLDPWD", oldpwd) != 0)
	{
		free_ptr(oldpwd);
		return (1);
	}
	free_ptr(oldpwd);
	//update PWD env variable
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("cd: getcwd failed");
		return (1);
	}
	if (set_env_var(msh, "PWD", cwd) != 0)
	{
		free_ptr(cwd);
		return (1);
	}
	free_ptr(cwd);
	return (0);
}

//helper function to change directories
int	change_dir(const char *path, t_msh *msh)
{
	char	*oldpwd;

	//store current directory as oldpwd
	oldpwd = getcwd(NULL, 0); //special feature of the getcwd function, when pointer is NULL it allocates memory with malloc to the size of the path, and size (0) argument is ignored. 
	if (!oldpwd)
	{
		perror("cd: getcwd failed");
		return (1);
	}
	if (chdir(path) != 0) //atempt to change dire and print message if it fails
	{
		perror("cd");
		free_ptr(oldpwd);
		return (1);
	}
	//use helper function to update the env variables OLDPWD and PWD
	if (update_env_change_dir(oldpwd, msh) != 0)
		return (1);
	return (0);
}

//main funtion do deal with cd builtin
int	execute_cd(t_msh *msh, char **args) //later we can improve how we deal with error messages in a centralized way and reduce the size of the function
{
	char	*path;

	if (!args[1] || args[1][0] == '\0' || ft_isspace(args[1][0]) || ft_strncmp(args[1], "--", 3) == 0)
	{
		path = get_env_var_value(msh->envp, "HOME");
		if (!path || !*path)
		{
			perror("-bash: cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_var_value(msh->envp, "OLDPWD");
		if (!path)
		{
			perror("-bash: cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", path);
	}
	else if (args[2])
	{
		perror("-bash: cd: too many arguments\n");
		return (1);
	}
	else
		path = args[1];
	return (change_dir(path, msh));
}

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
	printf("\n");
}
/*
//value attribution to my false envp array, define cd arguments
int	main(int argc, char **argv, char **envp)
{
	//creating a struct, an array of env vars, an array or cd arguments, and a counter var (i)
	t_msh	msh;
	char	*test_envp[] = {
			"HOME=/home/rmendes",
			"OLDPWD=/tmp",
			"PWD=/home/test_user",
			NULL	
			};
	char	*args[3];
	int		env_count, i = 0;
	(void)argc;
	(void)argv;
	(void)envp;

    // Initialize msh.envp
    msh.envp = NULL;
    env_count = env_var_count(test_envp);
    msh.envp = realloc_env_vars(&msh, env_count);
    while (test_envp[i])
    {
        msh.envp[i] = ft_strdup(test_envp[i]);
        i++;
    }

	//print inicial envp array to check if everything is ok
	printf("Initial Env Variables\n");
	print_envp(msh.envp);

	// Test 1: simple change
	printf("\nTest 1: simple directory change\n");
	args[0] = "cd";
	args[1] = "/tmp";
	args[2] = NULL;
	execute_cd(&msh, args);
	print_envp(msh.envp);

	// Test 2: Change to HOME directory
	printf("\nTest 2: cd with no arguments (HOME)\n");
	args[0] = "cd";
	args[1] = NULL;
	execute_cd(&msh, args);
	print_envp(msh.envp);

	// Test 3: Use OLDPWD
	printf("\nTest 3: cd -\n");
	args[0] = "cd";
	args[1] = "-";
	args[2] = NULL;
	execute_cd(&msh, args);
	print_envp(msh.envp);

	// Test 4: Invalid directory
	printf("\nTest 4: cd to non-existent directory\n");
	args[0] = "cd";
	args[1] = "/nonexistent";
	args[2] = NULL;
	execute_cd(&msh, args);
	print_envp(msh.envp);

	// Test 5: Too many arguments
	printf("\nTest 5: cd with too many arguments\n");
	args[0] = "cd";
	args[1] = "/tmp";
	args[2] = "/home";
	execute_cd(&msh, args);
	print_envp(msh.envp);

	// Test 6: Empty argument
	printf("\nTest 6: cd with empty argument\n");
	args[0] = "cd";
	args[1] = "";
	args[2] = NULL;
	execute_cd(&msh, args);
	print_envp(msh.envp);

	// Test 7: Double dash
	printf("\nTest 7: cd --\n");
	args[0] = "cd";
	args[1] = "--";
	args[2] = NULL;
	execute_cd(&msh, args);
	print_envp(msh.envp);

	//cleanup functions
	i = 0; 
	while (msh.envp[i])
	{
		free_ptr(msh.envp[i]);
		i++;
	}
	free_ptr(msh.envp);
	return (0);
}
*/

