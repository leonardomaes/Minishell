/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 01:25:10 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/02/15 00:58:10 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//used a simple buble sort to sort envp array (e.g. export builtin)
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

//function to free an array (used whith envp in export)
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

//check if env variable name is valid (export & unset)
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

//utility functions to join 4 strings (e.g. export builtin)
char	*ft_strjoin4(char *s1, char *s2, char *s3, char *s4)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	tmp = ft_strjoin(result, s4);
	free(result);
	result = tmp;
	return (result);
}
