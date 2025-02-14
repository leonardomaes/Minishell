/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:14 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 12:06:03 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//standard funtion to free a pointer of any type (avoids double free)
void	free_ptr(void **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

//print error, cleans and exits
void	ft_exit(t_msh *msh, int exit_code, char *msg_err, char *arg)
{
	if (msg_err != NULL)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(msg_err, 2);
		free(arg);
	}
	ft_free_all(msh);
	exit(exit_code);
}

void	ft_perror(t_msh *msh, char *msg, int exit_code)
{
	ft_free_all(msh);
	perror(msg);
	exit(exit_code);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	free_array(char **str, unsigned int n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
