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

/************** LIBRARIES **************/

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <termcap.h>
# include <termios.h>
# include <signal.h>
# include "includes/libft/libft.h"
# include <fcntl.h>

/************** DEFINES **************/

# define CMD
# define IN
# define OUT

// Builtins defines
# define BLT_ECHO 101
# define BLT_CD 102
# define BLT_PWD 103
# define BLT_EXPORT 104
# define BLT_UNSET 105
# define BLT_ENV 106
# define BLT_EXIT 107

// In type defines
# define DBL_QUOTES 108
# define SNG_QUOTES 109
# define TKN_PIPE 110
# define VAR_ENVIRON 111
# define ARGUMENT 112
# define TKN_OUTREDIR 113
# define TKN_INREDIR 114
# define TKN_APPEND 115
# define TKN_HEREDOC 116
# define TKN_BCMD 117
# define TKN_SPACE 118
# define TKN_DIR 119

// Exit messages
# define ERR_DIR	201
# define ERR_COMM_N_FOUND 202
# define ERR_SUCH_FILE 203
# define ERR_PERM_DENIED 204

// Macros for signal modes
# define SHELL_MODE		1
# define COMMAND_MODE	2
# define CHILD_MODE		3
# define EXIT			4
# define HEREDOC		5
# define HEREDOC_PAUSE	6

/********** GLOBAL VARIABLE **********/

extern int	g_exit;

/************** STRUCTS **************/

//struct for var expansion | start: after $ | end: first char after var name
typedef struct s_pos
{
	const char	*start;
	const char	*end;
}	t_pos;

// Split tokens struct
typedef struct s_split
{
	int	words;
	int	i;
	int	l;
}				t_split;

//token struct list
typedef struct s_tokens
{
	char				*name;
	char				**args;
	int					type;
	int					count;
	struct s_tokens		*prev;
	struct s_tokens		*next;
}				t_tokens;

//data struct to store info about args received
typedef struct s_data
{
	char		**cmd_paths;
	char		**args;
	int			argc;
	int			pipes;
	int			stdin_backup;
	int			stdout_backup;
	int			infile;
	int			outfile;
	int			prev_pipe;
	t_tokens	*tokens;
}				t_data;

//main minishell struct
typedef struct s_msh
{
	char	**envp;
	t_data	*data;
}				t_msh;

/************* FUNCTIONS *************/

/* MAIN */
void		ft_init_shell(t_msh **msh, char **envp);

/* READLINE */
char		*ft_prompt(void);
int			get_redir(t_tokens *temp);
int			ft_init_data(char *line, t_msh *msh);
int			ft_readline(t_msh *msh);

/* SYNTAX CHECK */
int			syntax_redirs(t_msh *msh, t_tokens *tokens);
int			syntax_pipes(t_tokens *tokens);
int			syntax_quotes(t_tokens *tokens);
int			syntax_check(t_msh *msh, t_data *data);

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

/* FREE 2 */
void		free_args(char **args);
void		ft_exit(t_msh *msh, int exit_code, char *msg_err, char *arg);
void		ft_print_error(char *msg_err, char	*arg, int should_free);
int			ft_countargs(char **args);
void		ft_perror(t_msh *msh, char *msg, int exit_code);

/****** PARSER AND TOKENS ******/
/* TOKENIZER */
void		split_tokens(t_msh *msh, t_tokens **token, t_tokens *prev, int i);
int			ft_isdelimiter(char c);
int			ft_isredirection(char c);
int			ft_isdelimiter(char c);
int			get_delimiter(t_msh *msh, char *data_args);

/* GET ARGS */
void		ft_get_args(t_msh *msh);
void		getargs(t_msh *msh, t_tokens **token);
char		**alloc_args(int words, int *len);
void		get_args(t_msh *msh, t_tokens	**tokens);

/* GET ARGS 2 */
int			alloc_getargs(t_msh *msh, t_tokens *token);
int			get_delimiter(t_msh *msh, char *data_args);
char		*return_arg(t_tokens *token);
char		*merge_args(t_tokens **temp);
void		ft_skip_delimiters(t_msh *msh, t_tokens **temp);

/* SPLIT TOKENS */
int			*calculate_lengths(t_msh *msh, const char *s, int words);
void		calculate_length(t_msh *msh, const char *s,
				t_split *nums, int **len);
char		**ft_split_args(t_msh *msh, const char *s);
void		split_arg(t_msh *msh, char ***str, const char *s, t_split *nums);

/* COUNT ARGS */
void		count_quotes(const char *s, int *i);
void		skip_spaces(const char *s, int *i);
void		count_dollar(const char *s, int *i);
int			count_args(const char *s);

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
int			open_files(t_msh *msh, t_tokens *token);
int			close_files(t_msh *msh);
char		*find_last_arg(t_tokens *temp);

/* EXECUTER */
int			exec_builtin(t_msh *msh, t_tokens *tokens);
void		handle_heredocs(t_msh *msh, t_tokens *token);
void		setup_heredocs(t_tokens *tokens, t_msh *msh);
int			execute_one(t_msh *msh, char **envp);
int			execute_multi(t_msh *msh);
int			execute(t_msh *msh);
void		ft_exec(t_msh *msh, t_tokens *tokens, char **envp);
void		skip_redirs(t_tokens **temp);
char		*ft_get_bcmd(char *cmd);

/* EXECUTER PIPES*/
int			execute_cmd(t_msh *msh, t_tokens *tokens, char **envp);
void		ft_parent_multi2(t_msh *msh, pid_t pid, int prev_pipe);
void		ft_child_process(t_msh *msh, t_tokens *cur, int *pipefd, int i);
int			execute_multi(t_msh *msh);

/* EXECUTER UTILS */
int			handle_redirs_one(t_msh *msh, t_tokens *token);
int			handle_redirs_multi(t_msh *msh, t_tokens *token, int prev_pipe);

/* HEREDOC */
void		handle_heredoc(t_tokens *token, t_msh *msh);

/* HEREDOC EXPANDER */
int			calculate_expanded_length(t_msh *msh, const char *line);
char		*expand_env_variables(t_msh *msh, const char *line);

/* BUILTINS | CD */
int			update_env_change_dir(char *oldpwd, t_msh *msh);
int			change_dir(const char *path, t_msh *msh);
int			execute_cd(t_msh *msh, char **args);

/* BUILTINS | ECHO */
int			execute_echo(t_msh *msh, char **args);

/* BUILTINS | ENV */
int			execute_env(char **args, char **envp);

/* BUILTINS | EXIT */
long long	ft_safe_atol(const char *str, int *error);
int			execute_exit(t_msh *msh, char **args);

/* BUILTINS | EXPORT */
int			print_sorted_env(char **envp);
int			execute_export(t_msh *msh, char **args);

/* BUILTINS | PWD */
int			execute_pwd(void);

/* BUILTINS | UNSET */
int			remove_env_var(t_msh *msh, char *var);
int			execute_unset(t_msh *msh, char **args);

/* BUILTINS | UTILS */
char		**realloc_env_vars(t_msh *msh, int size);
int			get_env_var_index(char **envp, char *var_name);
char		*get_env_var_value(char **envp, char *var);
int			set_env_var(t_msh *msh, char *var_name, char *var_value);

/* BUILTINS | UTILS 2 - EXPORT */
void		ft_sort_array(char **array, int count);
void		ft_print_array(char **array);
void		ft_free_array(char **array);
int			is_valid_var_name(char *name);
char		*ft_strjoin4(char *s1, char *s2, char *s3, char *s4);

/* BUILTINS | UTILS 3 - EXIT/EXPORT */
int			ft_isnumber(const char *str);
int			env_var_count(char **envp);
int			is_append_operation(const char *str);
char		*get_var_name(const char *str);
char		*get_append_value(const char *str);

/* SIGNAL HANDLING */
void		ft_sigint_shell(int sig);
void		ft_sigint_heredoc(int sig);
void		set_signal(int sg, t_msh *msg);
void		ft_sigint_child(int sig);

/* TRASH */
void		ft_print_splitargs(char **args);
void		ft_print_params(t_msh *msh);
void		ft_print_tokens(t_tokens *tokens);

#endif
