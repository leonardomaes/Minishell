/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:14 by lmaes             #+#    #+#             */
/*   Updated: 2025/01/11 02:25:53 by rda-cunh         ###   ########.fr       */
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
#include <fcntl.h>
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
#define ARGUMENT 112
#define TKN_HEREDOC 113

//macros for signal modes
#define SHELL_MODE		1
#define	COMMAND_MODE	2
#define EXIT			3
#define HEREDOC			4
#define HEREDOC_PAUSE	5

/********** GLOBAL VARIABLE **********/
extern int	g_exit;

/************* STRUCTS *************/
//msh->data->tokens

typedef struct s_expand // struct to deal with the variable expansion in heredoc
{
	char	*end;
	char	*new;
	int		i;
	int		start;
	int		position;
}				t_expand;

typedef struct s_tokens			// Struct de tokens (Ainda a implementar)
{
	char				*name;
	char				**args;
	int					type;
	int					count; // A principio somente para imprimir, nenhum uso
	struct s_tokens		*prev;
	struct s_tokens		*next;
}				t_tokens;

typedef struct s_data			// Info sobre argumentos recebidos
{
	char		**args;			// Vetor de strings com agrs
	int			argc;			// Quantidade de **args
	int			pipes;			// Quantidade de pipes
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
void		free_ptr(void **ptr);

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
int			exec_builtin(t_msh *msh);
void		handle_redirections(t_tokens *token);
void		setup_heredocs(t_tokens *tokens, t_msh *msh);
int			execute_one(t_msh *msh, char **envp);
void		executecmd(t_msh *msh);
int			execute(t_msh *msh);
int			execute_multi(t_msh *msh);

/* HEREDOC */
int			has_expand(const char *line);
int			search_variable_end(const char *s, int i);
void		get_expand_variable(char *line, t_msh *msh, t_expand *exp);
char		*expand_line(char *line, t_msh *msh);
void		handle_heredoc(char *delimiter, t_msh *msh);

/* BUILTINS | CD */
int			update_env_change_dir(char *oldpwd, t_msh *msh);
int			change_dir(const char *path, t_msh *msh);
int			execute_cd(t_msh *msh, char **args);

/* BUILTINS | ECHO */
int			execute_echo(char **args);

/* BUILTINS | ENV */
void		execute_env(char **envp);

/* BUILTINS | EXIT */
int			ft_isnumber(const char *str);
long long	ft_safe_atol(const char *str, int *error);
int			execute_exit(t_msh *msh, char **args);

/* BUILTINS | EXPORT */
void		ft_sort_array(char **array, int count);
void		ft_print_array(char **array);
void		ft_free_array(char **array); //evaluate to change it to free.c
int			is_valid_var_name(char *name);
int			print_sorted_env(char **envp);
int			execute_export(t_msh *msh, char **args);

/* BUILTINS | PWD */
int			execute_pwd(void);

/* BUILTINS | UNSET */
int			remove_env_var(t_msh *msh, char *var);
int			execute_unset(t_msh *msh, char **args);

/* BUILTINS | UTILS */
char		**realloc_env_vars(t_msh *msh, int size);
int			env_var_count(char **envp);
int			get_env_var_index(char **envp, char *var_name);
char		*get_env_var_value(char **envp, char *var);
int			set_env_var(t_msh *msh, char *var_name, char *var_value);

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
void	ft_print_tokens(t_tokens *tokens);

#endif
