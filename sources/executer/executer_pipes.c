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
	char 	*comm;
	char 	*cwd;
	int 	status;
	struct stat filestat;

	cwd = NULL;
	if (tokens->type >= 101 && tokens->type <= 107)
	{
		status = exec_builtin(msh, tokens);
		ft_free_all(msh);
		exit(status);
	}
	else
	{
		if (tokens->type == TKN_BCMD && (tokens->name[0] == '.' || tokens->name[0] == '/'))
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
		if (execve(comm, tokens->args, envp) == -1)
		{
			perror("execve:");
			free(comm);
			ft_free_all(msh);
			exit(126);
		}
		free(comm);
	}
	exit(1);
	return (0);
}

int	execute_multi(t_msh *msh)
{
	int		pipefd[2];
	int		prev_pipe;
	int		status;
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
		{
			perror("pipe: ");
			ft_free_all(msh);
			exit(1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork: ");
			ft_free_all(msh);
			exit(1);
		}
		else if (pid == 0) // Processo filho
		{
			set_signal(CHILD_MODE, msh);
			//printf("-%s\n", current_token->name);
			if (open_files(msh, current_token) != 0)
			{
				g_exit = 1;
				ft_free_all(msh);
				exit(g_exit);
			}
			// Redirecionamento de Entrada (Se houver infile ou pipe anterior)
			if (prev_pipe != -1) 
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (msh->data->infile > 0) // Se houver infile aberto
			{
				dup2(msh->data->infile, STDIN_FILENO);
				close(msh->data->infile);
			}
			if (msh->data->outfile > 0) // Se houver outfile aberto
			{
				dup2(msh->data->outfile, STDOUT_FILENO);
				close(msh->data->outfile);
			}
			// Redirecionamento de Saída (Se houver outfile ou próximo comando no pipeline)
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
		}
		// Processo pai
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
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))   //if it receives a signal in child
	{	
		g_exit = 128 + WTERMSIG(status); //manual change g_exit to 128 + signal number
		if (WTERMSIG(status) == SIGINT) //manual handling error because SIGINT was not printing the line break 
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
	while (i-- > 0)
		waitpid(-1, NULL, 0);
	set_signal(SHELL_MODE, msh);
	return (0);
}
