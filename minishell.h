/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:14 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/09 18:45:58 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHEL_H
# define MINISHEL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <termcap.h>
#include <termios.h>
#include <signal.h>
#include "includes/libft/libft.h"

/************* DEFINES *************/
#define CMD
#define IN
#define OUT

// Builtins defines
#define BLT_ECHO 101
#define BLT_CD 102
#define BLT_PWD 103
#define BLT_EXPORT 104
#define BLT_UNSET 105
#define BLT_ENV 106
#define BLT_EXIT 107

// In type defines
#define DBL_QUOTES 108
#define SNG_QUOTES 109
#define TKN_PIPE 110
#define VAR_ENVIRON 111

//macros for signal modes
#define SHELL_MODE		1
#define	COMMAND_MODE	2
#define EXIT			3
#define HEREDOC			4
#define HEREDOC_PAUSE	5

/********** GLOBAL VARIABLE **********/
extern int	g_signal;

/************* STRUCTS *************/
//msh->data->tokens

typedef struct s_tokens			// Struct de tokens (Ainda a implementar)
{
	char				*name;
	int					type;
	int					count;
	struct s_tokens		*prev;
	struct s_tokens		*next;
}				t_tokens;

typedef struct s_data			// Info sobre argumentos recebidos
{
	int			argc;			// Quantidade de **args
	char		**args;			// Vetor de strings com agrs
	t_tokens	*tokens;
}				t_data;

typedef struct s_msh			// Main struct que contem tudo
{
	char	**envp;
	char	**cmd_paths;
	t_data	*data;
}               t_msh;


/************* FUNCTIONS *************/

/* INIT */
void		ft_init_shell(t_msh **msh, char **envp);

/* READLINE */
char		*ft_prompt();
void		ft_readline(t_msh *msh);


/* ENVIRON */
char		*expand_env(char **envp, char *name);
char		*ft_get_path(char **envp);
char		**ft_get_env(char **envp);
char		*ft_get_command(char *cmd, char **path);

/* FREE */
void		ft_free_tokens(t_tokens *tokens);
void		ft_free_all(t_msh *msh);
void		ft_free_data(t_msh	*msh);
void		free_array(char **str, unsigned int n);

/* PARSER AND TOKENS */
int			ft_countargs(char **args);
int			ft_parsing(t_msh *msh, char *line);
int			get_builtin_type(char *name);
int			get_meta_type(char *name);
int			get_type(char *name);
char		**ft_split_args(const char *s);
void		split_tokens(t_msh *msh, t_tokens **token, t_tokens *prev, int i);

/* TOKEN UTILS */
int			handle_single_quote(const char **s, char *str);
int			handle_double_quote(const char **s, char *str);
int			double_quote_lenght(const char *s, int *i);
int			single_quote_lenght(const char *s, int *i);
int			environ_lenght(const char *s, int *i);
int			handle_environ(const char **s, char *str);

/* EXECUTER */
int			execute(t_msh *msh);

/* BUILTINS */
void		exec_env(char **envp);
int			execute_echo(char **args);

/* SIGNAL HANDLING */
void		ft_sigint_shell(int sig);
void		ft_sigint_command(int sig);
void		ft_sigquit(int sig);
void		child_signal_handler(int sig);
void		child_signal_handler2(int sig);
void		set_signal(int sg, t_msh *msg);

/* TRASH */
void		ft_print_splitargs(char **args);
void		ft_print_params(t_msh *msh);
void		ft_print_tokens(t_msh *msh);

#endif
