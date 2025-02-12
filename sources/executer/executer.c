/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:20:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/13 19:20:01 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* int	ft_isdirectory(t_msh *msh, t_tokens *tokens)
{
	struct stat filestat;
	char *comm;
	char *cwd;
	char	*temp;

	cwd = getcwd(NULL, 0);
	
} */

void	ft_exec(t_msh *msh, t_tokens *tokens, char **envp)
{
	char	*comm;
	char	*cwd;
	struct stat filestat;

	if (tokens->type == TKN_BCMD && (tokens->name[0] == '.' && tokens->name[1] == '/'))
	{
		cwd = getcwd(NULL, 0);
		comm = ft_strjoin(cwd, tokens->name + 1);
		free(cwd);
	}
	else
		comm = ft_get_command(msh, tokens->args[0], msh->data->cmd_paths);
	if (stat(comm, &filestat) == 0 && S_ISDIR(filestat.st_mode))
		ft_exit(msh, 126, ": Is a directory\n", comm);
	if (!comm)
		ft_exit(msh, 127, ": command not found\n", ft_strdup(tokens->args[0]));
	if (access(comm, F_OK) == -1)
		ft_exit(msh, 127, ": No such file or directory\n", comm);
	if (access(comm, X_OK) == -1)
		ft_exit(msh, 126, ": Permission denied\n", comm);
	if (execve(comm, tokens->args, envp) == -1)
	{
		free(comm);
		ft_perror(msh, "execve:", 126);
	}
}

void	ft_parent(t_msh *msh, pid_t pid)
{
	int status;

	set_signal(COMMAND_MODE, msh);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{	
		g_exit = 128 + WTERMSIG(status); //manual change g_exit to 128 + signal number
		if (WTERMSIG(status) == SIGINT) //manual handling error because SIGINT was not printing the line break 
			write(1, "\n", 1);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit\n", 2);
	}
	else if (WIFEXITED(status))
		g_exit = WEXITSTATUS(status);
	set_signal(SHELL_MODE, msh);
}

int execute_one(t_msh *msh, char **envp)
{
	pid_t 	pid;

	g_exit = 0;
	setup_heredocs(msh->data->tokens, msh);
	if (msh->data->tokens->type == TKN_HEREDOC)
		return (0);
	handle_redirs(msh, msh->data->tokens, -1);
	if (msh->data->tokens->type >= 101 && msh->data->tokens->type <= 107)
		g_exit = exec_builtin(msh, msh->data->tokens);
	else
	{
		pid = fork();
		if (pid == -1)
			ft_perror(msh, "fork: ", 1);
		else if (pid == 0)
		{
			set_signal(CHILD_MODE, msh);
			handle_heredocs(msh, msh->data->tokens);
			ft_exec(msh, msh->data->tokens, envp);
		}
		else
			ft_parent(msh, pid);
	}
	return (0);
}

int execute(t_msh *msh)
{
	int status;

	status = 1;
	if (msh->data->pipes == 0)
		status = execute_one(msh, msh->envp);
	else
		status = execute_multi(msh);
	close_files(msh);
	return (status);
}
