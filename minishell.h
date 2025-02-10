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

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <termcap.h>
#include <termios.h>
#include <signal.h>
#include "includes/libft/libft.h"
#include <fcntl.h>

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
#define TKN_OUTREDIR 113
#define TKN_INREDIR 114
#define TKN_APPEND 115
#define TKN_HEREDOC 116
#define TKN_BCMD 117
#define TKN_SPACE 118
#define TKN_DIR 119

//macros for signal modes
#define SHELL_MODE		1
#define	COMMAND_MODE	2
#define	CHILD_MODE		3
#define EXIT			4
#define HEREDOC			5
#define HEREDOC_PAUSE	6

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
	char		**cmd_paths;
	char		**args;			// Vetor de strings com agrs
	int			argc;			// Quantidade de **args
	int			pipes;			// Quantidade de pipes
	int			stdin_backup;
	int			stdout_backup;
	int			infile;
	int			outfile;
	t_tokens	*tokens;
}				t_data;

typedef struct s_msh			// Main struct que contem tudo
{
	char	**envp;
	t_data	*data;
}               t_msh;


/************* FUNCTIONS *************/

/* INIT */
void		ft_init_shell(t_msh **msh, char **envp);

/* READLINE */
int			ft_readline(t_msh *msh);
int			syntax_check(t_msh *msh, t_data *data);
int			ft_countargs(char **args);
char		*ft_prompt(void);

/* ENVIRON */
char		*expand_env(char **envp, char *name);
char		*ft_get_path(char **envp);
char		**ft_get_env(char **envp);
char		*ft_get_command(t_msh *msh, char *cmd, char **path);

/* FREE */
void		ft_free_tokens(t_tokens *tokens);
void		ft_free_all(t_msh *msh);
void		ft_free_data(t_msh	*msh);
void		free_array(char **str, unsigned int n);
void		free_ptr(void **ptr);
void		free_args(char **args);

/****** PARSER AND TOKENS ******/
/* TOKENIZER */
char		**alloc_args(int words, int *len);
char		**get_args(char **data_args, int i, t_msh *msh);
void		split_tokens(t_msh *msh, t_tokens **token, t_tokens *prev, int i);
int			ft_isdelimiter(char c);
int			ft_isredirection(char c);
int			ft_isdelimiter(char c);
int			get_delimiter(t_msh *msh, char *data_args);

/* GET ARGS */
void		ft_get_args(t_msh *msh);
char		**getargs(t_msh *msh, t_tokens *token);
/* SPLIT TOKENS */
int			count_args(const char *s);
int			*calculate_lengths(t_msh *msh, const char *s, int words);
char		**ft_split_args(t_msh *msh, const char *s);

/* TOKEN UTILS */
int			double_quote_lenght(t_msh *msh, const char *s, int *i);
int			single_quote_lenght(const char *s, int *i);
int			environ_lenght(t_msh *msh, const char *s, int *i);
int			handle_environ(t_msh *msh, const char *s, char *str, int *l);
int			handle_single_quote(const char *s, char *str, int *l);
int			handle_double_quote(t_msh *msh, const char *s, char *str, int *l);
char		*ft_chartrim(char **s, char set);

/* TOKEN TYPE */
int			get_builtin_type(char *name);
int			get_meta_type(t_msh *msh, char *name, int i);
int			get_type(t_msh *msh, char *name, int i);

/* REDIRECTIONS */
void		handle_redirs(t_msh *msh, int i, t_tokens *temp);
int			open_files(t_msh *msh, t_tokens *token);
int			close_files(t_msh *msh);
/* EXECUTER */
int			exec_builtin(t_msh *msh, t_tokens *tokens);
void		handle_redirections(t_msh *msh, t_tokens *token);
void		setup_heredocs(t_tokens *tokens, t_msh *msh);
int			execute_cmd(t_msh *msh, t_tokens *tokens, char **envp);
int			execute_one(t_msh *msh, char **envp);
int			execute_multi(t_msh *msh);
int			execute(t_msh *msh);

/* HEREDOC */
int			has_expand(const char *line);
int			search_variable_end(const char *s, int i);
void		get_expand_variable(char *line, t_msh *msh, t_expand *exp);
char		*expand_line(char *line, t_msh *msh);
void		handle_heredoc(t_tokens *token, t_msh *msh);

/* BUILTINS | CD */
int			update_env_change_dir(char *oldpwd, t_msh *msh);
int			change_dir(const char *path, t_msh *msh);
int			execute_cd(t_msh *msh, char **args);

/* BUILTINS | ECHO */
int			execute_echo(t_msh *msh, char **args);

/* BUILTINS | ENV */
int			execute_env(char **args, char **envp);

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
void		set_signal(int sg, t_msh *msg);

/* TRASH */
void		ft_print_splitargs(char **args);
void		ft_print_params(t_msh *msh);
void		ft_print_tokens(t_tokens *tokens);

#endif
