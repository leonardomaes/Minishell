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
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(msg_err, 2);
	free(arg);
	ft_free_all(msh);
	exit(exit_code);
}

//print error (do not free and exit)
void	ft_print_error(char *msg_err, char	*arg, int should_free)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(msg_err, 2);
	ft_putstr_fd("\n", 2);
	if (should_free)
		free(arg);
}

//counts the number of args in a array
int	ft_countargs(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}
