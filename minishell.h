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
	int		pipes;
	t_tokens	*tokens;
}				t_data;

typedef struct s_msh
{
	char	**envp;
	char	*argv;
	t_data	*data;
}               t_msh;

typedef struct s_tokens
{
	char		*name;
	char		*type;
	t_tokens 	*left;
	t_tokens 	*right;
}				t_tokens;




/************* FUNCTIONS *************/

/* MAIN */


/* INIT */
char	**get_env(char **envp);
void init_shell(t_msh **msh, char **envp);

/* READLINE */
t_data  *ft_readline(void);




/* TRASH */
void	print_envp(char **envp);


/* PARSER */
t_tokens *parsing(char **args);


#endif
