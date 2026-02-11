/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_pipes2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:20:00 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 23:08:12 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_cmd(t_msh *msh, t_tokens *tokens, char **envp)
{
	int		status;

	if (tokens->type == TKN_HEREDOC)
		skip_redirs(&tokens);
	if (tokens && tokens->type >= 101 && tokens->type <= 107)
	{
		status = exec_builtin(msh, tokens);
		ft_free_all(msh);
		exit(status);
	}
	else
		ft_exec(msh, tokens, envp);
	return (0);
}

void	ft_exec(t_msh *msh, t_tokens *tokens, char **envp)
{
	char		*comm;
	struct stat	filestat;

	if (tokens == NULL || tokens->type == TKN_SPACE)
		ft_exit(msh, 0, NULL, NULL);
	if (tokens->type == TKN_BCMD && (tokens->name[0] == '.'
			&& tokens->name[1] == '/'))
		comm = ft_get_bcmd(tokens->name + 2);
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

void	ft_parent_multi2(t_msh *msh, pid_t pid, int prev_pipe)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		g_exit = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit\n", 2);
		if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("\n", 2);
	}
	else if (WIFEXITED(status))
		g_exit = WEXITSTATUS(status);
	if (msh->data->infile > 0)
	{
		dup2(msh->data->stdin_backup, STDIN_FILENO);
		close(msh->data->stdin_backup);
	}
	if (msh->data->outfile > 0)
	{
		dup2(msh->data->stdout_backup, STDOUT_FILENO);
		close(msh->data->stdout_backup);
	}
	if (prev_pipe != -1)
		close(prev_pipe);
}

void	ft_child_process(t_msh *msh, t_tokens *cur, int *pipefd, int i)
{
	set_signal(CHILD_MODE, msh);
	if (i < msh->data->pipes && msh->data->outfile == -1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (msh->data->prev_pipe != -1)
		close(msh->data->prev_pipe);
	handle_heredocs(msh, cur);
	while (cur && get_delimiter(msh, cur->name) != 0)
		skip_redirs(&cur);
	while (cur && cur->type == TKN_SPACE)
		cur = cur->next;
	execute_cmd(msh, cur, msh->envp);
	exit(1);
}
