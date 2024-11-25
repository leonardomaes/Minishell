/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:14 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/07 12:50:16 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHEL_H
# define MINISHEL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <termcap.h>
#include <termios.h>
#include "includes/libft/libft.h"

typedef struct s_data
{
	char	*argv;
}				t_data;

typedef struct s_msh
{
	char	**envp;
	t_data	*data;
}               t_msh;




/************* FUNCTIONS *************/

/* MAIN */


/* INIT */
char	**get_env(char **envp);
t_msh	*init_shell(char **envp);

/* READLINE */
t_data  *ft_readline(void);




/* TRASH */
void	print_envp(char **envp);


#endif
