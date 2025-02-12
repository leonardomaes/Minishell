/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:20:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/12/13 19:20:01 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_cmd(t_msh *msh, t_tokens *tokens, char **envp)
{
	int		status;

	if (tokens->type >= 101 && tokens->type <= 107)
	{
		status = exec_builtin(msh, tokens);
		ft_free_all(msh);
		exit(status);
	}
	else
		ft_exec(msh, tokens, envp);
	return (0);
}

void	ft_parent_multi2(t_msh *msh, pid_t pid, int prev_pipe)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		g_exit = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit\n", 2);
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
	handle_heredocs(msh, cur);
	execute_cmd(msh, cur, msh->envp);
	exit(1);
}

int	execute_multi(t_msh *msh)
{
	int			pipefd[2];
	int			prev_pipe;
	int			i;
	pid_t		pid;
	t_tokens	*cur;

	g_exit = 0;
	i = 0;
	prev_pipe = -1;
	cur = msh->data->tokens;
	while (i <= msh->data->pipes)
	{
		setup_heredocs(cur, msh);
		if (i < msh->data->pipes && pipe(pipefd) == -1)
			ft_perror(msh, "pipe: ", 1);
		pid = fork();
		if (pid == -1)
			ft_perror(msh, "fork: ", 1);
		else if (pid == 0)
		{
			handle_redirs(msh, cur, prev_pipe);
			ft_child_process(msh, cur, pipefd, i);
		}
		set_signal(COMMAND_MODE, msh);
		if (prev_pipe != -1)
			close(prev_pipe);
		if (i < msh->data->pipes)
		{
			close(pipefd[1]);
			prev_pipe = pipefd[0];
		}
		close_files(msh);
		while (cur && cur->type != TKN_PIPE)
			cur = cur->next;
		if (cur && cur->type == TKN_PIPE)
			cur = cur->next;
		if (cur && cur->type == TKN_SPACE)
			cur = cur->next;
		i++;
	}
	ft_parent_multi2(msh, pid, prev_pipe);
	while (i-- > 0)
		waitpid(-1, NULL, 0);
	set_signal(SHELL_MODE, msh);
	return (0);
}
