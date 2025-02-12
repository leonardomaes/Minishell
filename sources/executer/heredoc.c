/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 00:09:07 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/02/12 02:30:00 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//child process: reads lines until delimiter and expands env vars
static void	heredoc_child(t_tokens *t, t_msh *msh, int fd)
{
	char	*line;

	set_signal(HEREDOC, msh);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, t->args[0]))
		{
			if (!line)
				ft_putstr_fd("warning: here-doc delimited by EOF\n", 2);
			free(line);
			break ;
		}
		if (t->next->type != SNG_QUOTES && t->next->type != DBL_QUOTES)
			line = expand_env_variables(msh, line);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	ft_free_all(msh);
	exit(0);
}

//parent process: waits for child, handle signals and exit
static void	heredoc_parent(t_msh *msh, int pid, int fd)
{
	int	status;

	set_signal(HEREDOC_PAUSE, msh);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) || WEXITSTATUS(status))
	{
		g_exit = 130;
		close(fd);
		return ;
	}
	close(fd);
	set_signal(SHELL_MODE, msh);
}

//main heredoc function: creates temp file and manage processes
void	handle_heredoc(t_tokens *token, t_msh *msh)
{
	int		fd;
	pid_t	pid;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		perror("heredoc");
		return ;
	}
	pid = fork ();
	if (pid == -1)
	{
		close(fd);
		perror("fork");
		return ;
	}
	if (pid == 0)
		heredoc_child(token, msh, fd);
	else
		heredoc_parent(msh, pid, fd);
}
