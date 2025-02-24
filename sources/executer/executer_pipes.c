/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:20:00 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 23:20:51 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_pipe_creation(t_msh *msh, int *pipefd, int i)
{
	if (i < msh->data->pipes && pipe(pipefd) == -1)
		ft_perror(msh, "pipe: ", 1);
}

static int	process_child(t_msh *msh, t_tokens *cur, int *pipefd, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_perror(msh, "fork: ", 1);
	else if (pid == 0)
	{
		handle_redirs_multi(msh, cur, msh->data->prev_pipe);
		if (!cur)
			ft_exit(msh, 0, NULL, NULL);
		ft_child_process(msh, cur, pipefd, i);
	}
	return (pid);
}

static void	update_pipe_status(t_msh *msh, int *pipefd, int i)
{
	if (msh->data->prev_pipe != -1)
		close(msh->data->prev_pipe);
	if (i < msh->data->pipes)
	{
		close(pipefd[1]);
		msh->data->prev_pipe = pipefd[0];
	}
}

static t_tokens	*move_to_next_command(t_tokens *cur)
{
	while (cur && cur->type != TKN_PIPE)
		cur = cur->next;
	if (cur && cur->type == TKN_PIPE)
		cur = cur->next;
	if (cur && cur->type == TKN_SPACE)
		cur = cur->next;
	return (cur);
}

int	execute_multi(t_msh *msh)
{
	int			pipefd[2];
	int			i;
	pid_t		pid;
	t_tokens	*cur;

	g_exit = 0;
	i = 0;
	msh->data->prev_pipe = -1;
	cur = msh->data->tokens;
	while (i <= msh->data->pipes)
	{
		setup_heredocs(cur, msh);
		handle_pipe_creation(msh, pipefd, i);
		pid = process_child(msh, cur, pipefd, i);
		set_signal(COMMAND_MODE, msh);
		update_pipe_status(msh, pipefd, i);
		close_files(msh);
		cur = move_to_next_command(cur);
		i++;
	}
	ft_parent_multi2(msh, pid, msh->data->prev_pipe);
	while (i-- > 0)
		waitpid(-1, NULL, 0);
	set_signal(SHELL_MODE, msh);
	return (0);
}
