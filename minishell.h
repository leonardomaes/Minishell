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

typedef struct s_tokens			// Struct de tokens (Ainda a implementar)
{
	char				*name;
	char				*type;
	struct s_tokens		*left;
	struct s_tokens		*right;
}				t_tokens;

typedef struct s_data			// Info sobre argumentos recebidos
{
	int			pipes;
	int			argc;
	char		**args;
	t_tokens	*tokens;
}				t_data;

typedef struct s_msh			// Main struct que contem tudo
{
	char	*argv;
	char	**envp;
	char	**cmd_paths;
	t_data	*data;
}               t_msh;


/************* FUNCTIONS *************/

/* MAIN */
void		ft_free_all(t_msh *msh);

/* INIT */
char		**ft_get_env(char **envp);
void		ft_init_shell(t_msh **msh, char **envp);

/* READLINE */
t_data		*ft_readline(t_msh *msh);


/* ENVIRON */
char		*ft_get_path(char **envp);
char		**ft_get_env(char **envp);
char		*ft_get_command(char *cmd, char **path);

/* PARSER */
int			ft_countargs(char **args);
void 		ft_parsing(t_msh *msh);


/* TRASH */
void		ft_print_splitargs(char **args);
void		ft_print_params(t_msh *msh, t_data *data);

#endif
