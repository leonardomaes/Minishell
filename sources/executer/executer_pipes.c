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

int execute_cmd(t_msh *msh, t_tokens *tokens, char **envp)
{
	int 	status;

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
	int status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{	
		g_exit = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit\n", 2);
	}
	else if(WIFEXITED(status))
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

void	ft_child_process(t_msh *msh, t_tokens *cur, int prev_pipe, int *pipefd, int i)
{
	set_signal(CHILD_MODE, msh);
	if (i < msh->data->pipes && msh->data->outfile == -1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	handle_redirs(msh, cur, prev_pipe);
	handle_heredocs(msh, cur);
	execute_cmd(msh, cur, msh->envp);
	exit(1);
}

int	execute_multi(t_msh *msh)
{
	int		pipefd[2], prev_pipe = -1, i = 0;
	pid_t	pid;
	t_tokens *cur = msh->data->tokens;

	g_exit = 0;
	while (i <= msh->data->pipes)
	{
		setup_heredocs(cur, msh);
		if (i < msh->data->pipes && pipe(pipefd) == -1)
			ft_perror(msh, "pipe: ", 1);
		pid = fork();
		if (pid == -1)
			ft_perror(msh, "fork: ", 1);
		else if (pid == 0)
			ft_child_process(msh, cur, prev_pipe, pipefd, i);
		set_signal(COMMAND_MODE, msh);
		if (prev_pipe != -1)
			close(prev_pipe);
		if (i < msh->data->pipes)
		{
			close(pipefd[1]); // Fecha a escrita do pipe
			prev_pipe = pipefd[0]; // Mantém a leitura do pipe para o próximo comando
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
/* 
int	execute_multi(t_msh *msh)
{
	int		pipefd[2];
	int		prev_pipe;
	int		i;
	pid_t 	pid;
	t_tokens *current_token;

	i = 0;
	prev_pipe = -1;
	g_exit = 0;
	current_token = msh->data->tokens;
	while (i <= msh->data->pipes)
	{
		setup_heredocs(current_token, msh);
		if (i < msh->data->pipes && pipe(pipefd) == -1) // Criando pipe para o próximo comando
			ft_perror(msh, "pipe: ", 1);
		pid = fork();
		if (pid == -1)
			ft_perror(msh, "fork: ", 1);
		else if (pid == 0) // Processo filho
		{
			// -------------------------
			set_signal(CHILD_MODE, msh);
			handle_redirs(msh, current_token, prev_pipe);
			if (i < msh->data->pipes && msh->data->outfile == -1) 
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				close(pipefd[0]);
			}
			// Executa o comando
			handle_heredocs(msh, current_token);
			execute_cmd(msh, current_token, msh->envp);
			exit(1);
			// -------------------------
		}
		// -------------------------
		set_signal(COMMAND_MODE, msh);
		if (prev_pipe != -1)
			close(prev_pipe);
		if (i < msh->data->pipes)
		{
			close(pipefd[1]); // Fecha a escrita do pipe
			prev_pipe = pipefd[0]; // Mantém a leitura do pipe para o próximo comando
		}
		close_files(msh);
		// Avança para o próximo comando
		while (current_token && current_token->type != TKN_PIPE)
			current_token = current_token->next;
		if (current_token && current_token->type == TKN_PIPE)
			current_token = current_token->next;
		if (current_token && current_token->type == TKN_SPACE)
			current_token = current_token->next;
		i++;
		// -------------------------
	}
	ft_parent_multi2(msh, pid, prev_pipe);
	while (i-- > 0)
		waitpid(-1, NULL, 0);
	set_signal(SHELL_MODE, msh);
	return (0);
}




int	execute_multi(t_msh *msh)
{
	int		pipefd[2];
	int		prev_pipe;
	int		i;
	pid_t 	pid;
	t_tokens *current_token;

	i = 0;
	prev_pipe = -1;
	g_exit = 0;
	current_token = msh->data->tokens;
	// Abre arquivos apenas no primeiro comando
	while (i <= msh->data->pipes)
	{
		setup_heredocs(current_token, msh);
		if (i < msh->data->pipes && pipe(pipefd) == -1) // Criando pipe para o próximo comando
			ft_perror(msh, "pipe: ", 1);
	
		pid = fork();
		if (pid == -1)
			ft_perror(msh, "fork: ", 1);
		else if (pid == 0) // Processo filho
		{
			// -------------------------
			set_signal(CHILD_MODE, msh);
			handle_redirs(msh, current_token, prev_pipe);
			if (i < msh->data->pipes && msh->data->outfile == -1) 
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				close(pipefd[0]);
			}
			// Executa o comando
			handle_heredocs(msh, current_token);
			execute_cmd(msh, current_token, msh->envp);
			exit(1);
			// -------------------------
		}
		ft_parent_multi1(msh, prev_pipe, pipefd, i);
		// -------------------------
		close_files(msh);
		// Avança para o próximo comando
		while (current_token && current_token->type != TKN_PIPE)
			current_token = current_token->next;
		if (current_token && current_token->type == TKN_PIPE)
			current_token = current_token->next;
		if (current_token && current_token->type == TKN_SPACE)
			current_token = current_token->next;
		i++;
		// -------------------------
	}
	ft_parent_multi2(msh, pid, prev_pipe);
	while (i-- > 0)
		waitpid(-1, NULL, 0);
	set_signal(SHELL_MODE, msh);
	return (0);
} */
