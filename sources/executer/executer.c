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

char	*ft_get_bcmd(char *cmd)
{
	char	*cwd;
	char	*temp;
	char	*comm;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	temp = ft_strjoin(cwd, "/");
	free(cwd);
	if (!temp)
		return (NULL);
	comm = ft_strjoin(temp, cmd);
	free(temp);
	return (comm);
}

void	skip_redirs(t_tokens **temp)
{
	*temp = (*temp)->next;
	if (*temp && (*temp)->type == TKN_SPACE)
		*temp = (*temp)->next;
	while (*temp && (*temp)->type != TKN_SPACE && (*temp)->type != TKN_PIPE)
		*temp = (*temp)->next;
	while (*temp && (*temp)->type == TKN_SPACE && (*temp)->type != TKN_PIPE)
		*temp = (*temp)->next;
}

void	ft_parent(t_msh *msh, pid_t pid)
{
	int	status;

	set_signal(COMMAND_MODE, msh);
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
	set_signal(SHELL_MODE, msh);
}

int	execute_one(t_msh *msh, char **envp)
{
	pid_t		pid;
	t_tokens	*tokens;

	tokens = msh->data->tokens;
	g_exit = 0;
	setup_heredocs(msh->data->tokens, msh);
	if (handle_redirs_one(msh, msh->data->tokens))
		return (-1);
	ft_skip_delimiters(msh, &tokens);
	if (!tokens)
		return (0);
	if (tokens->type >= 101 && tokens->type <= 107)
		g_exit = exec_builtin(msh, tokens);
	else
	{
		pid = fork();
		if (pid == -1)
			ft_perror(msh, "fork: ", 1);
		else if (pid == 0)
		{
			set_signal(CHILD_MODE, msh);
			handle_heredocs(msh, msh->data->tokens);
			ft_exec(msh, tokens, envp);
		}
		else
			ft_parent(msh, pid);
	}
	return (0);
}

int	execute(t_msh *msh)
{
	int	status;

	status = 1;
	if (msh->data->pipes == 0)
		status = execute_one(msh, msh->envp);
	else
		status = execute_multi(msh);
	close_files(msh);
	if (access(".heredoc_tmp", F_OK) == 0)
	{
		if (unlink(".heredoc_tmp") == -1)
			perror("heredoc cleanup failed");
	}
	return (status);
}
