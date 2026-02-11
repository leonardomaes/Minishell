/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 02:29:37 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/02/12 02:37:35 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//helper function calculates expanded lenght after variable expansion 
static int	get_var_value_length(t_msh *msh, const char *var_start,
	const char *var_end)
{
	char	*var_name;
	int		value_len;

	value_len = 0;
	var_name = ft_strndup(var_start, var_end - var_start);
	if (get_env_var_value(msh->envp, var_name))
		value_len = ft_strlen(get_env_var_value(msh->envp, var_name));
	free(var_name);
	return (value_len);
}

//processes line and calculate total lenght (including chars and vars expansion)
int	calculate_expanded_length(t_msh *msh, const char *line)
{
	int			i;
	int			new_len;
	t_pos		p;

	i = 0;
	new_len = 0;
	while (line[i])
	{
		if (line[i] == '$' && ft_isalnum(line[i + 1]))
		{
			p.start = &line[i + 1];
			p.end = p.start;
			while (*p.end && (ft_isalnum(*p.end) || *p.end == '_'))
				p.end++;
			new_len += get_var_value_length(msh, p.start, p.end);
			i += p.end - p.start + 1;
		}
		else
		{
			new_len++;
			i++;
		}
	}
	return (new_len);
}

//Copies each variable value to result buffer
static void	copy_expanded_value(t_msh *msh, const char *var_start,
	const char *var_end, char **result)
{
	char	*var_name;
	char	*var_value;
	int		k;

	var_name = ft_strndup(var_start, var_end - var_start);
	var_value = get_env_var_value(msh->envp, var_name);
	free(var_name);
	if (var_value)
	{
		k = 0;
		while (var_value[k])
			*(*result)++ = var_value[k++];
	}
}

//main function for managing var expansion
char	*expand_env_variables(t_msh *msh, const char *line)
{
	char		*result;
	char		*buf_ptr;
	t_pos		p;
	int			i;

	result = malloc(calculate_expanded_length(msh, line) + 1);
	if (!result)
		return (NULL);
	buf_ptr = result;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && ft_isalnum(line[i + 1]))
		{
			p.start = &line[i + 1];
			p.end = p.start;
			while (*p.end && (ft_isalnum(*p.end) || *p.end == '_'))
				p.end++;
			copy_expanded_value(msh, p.start, p.end, &buf_ptr);
			i += p.end - p.start + 1;
		}
		else
			*buf_ptr++ = line[i++];
	}
	return (*buf_ptr = '\0', result);
}
