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

int exec_builtin(t_msh *msh)
{
	if (msh->data->tokens->type == BLT_ECHO)
		execute_echo(msh->data->tokens->args);
	else if(msh->data->tokens->type == BLT_CD)
		execute_cd(msh, msh->data->tokens->args);
	else if(msh->data->tokens->type == BLT_PWD)
		execute_pwd();
	else if(msh->data->tokens->type == BLT_EXPORT)
		execute_export(msh, msh->data->tokens->args);
	else if(msh->data->tokens->type == BLT_UNSET)
		execute_unset(msh, msh->data->tokens->args);
	else if(msh->data->tokens->type == BLT_ENV)
		execute_env(msh->envp);
	else if(msh->data->tokens->type == BLT_EXIT)
		execute_exit(msh, msh->data->tokens->args);
	return (0);
}
int execute_one(t_msh *msh, char **envp)
{
	char *comm;
	pid_t pid;
	
	if (msh->data->tokens->type >= 101 && msh->data->tokens->type <= 107)
		exec_builtin(msh);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			comm = ft_get_command(msh->data->tokens->name, msh->cmd_paths);
			if (!comm)
			{
				printf("bash: %s: command not found\n", msh->data->tokens->name);
				free(comm);
				ft_free_all(msh);
					exit(127);
			}
			if (execve(comm, msh->data->tokens->args, envp) == -1)
			{
				perror("execve:");
				free(comm);
				ft_free_all(msh);
				exit(1);
			}
			free(comm);
		}
		else
		{
			waitpid(pid, NULL, 0);
			return (-1);
		}
	}
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
	current_token = msh->data->tokens;
	while (i <= msh->data->pipes)
	{
		if (i < msh->data->pipes && pipe(pipefd) == -1) // Se não for ultimo e der erro no pipe
		{
			perror("pipe:");
			exit(1);
		}
		pid = fork();
		if (pid == 0)
		{
			if (prev_pipe != -1) // Caso haja comando anterior, altera o input para o resultado anterior
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (i < msh->data->pipes) // Caso não for o ultimo comando, altera output para guardar no pipe
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				close(pipefd[0]);
			}
			msh->data->tokens = current_token;
			execute_one(msh, msh->envp);
			ft_free_all(msh);
			exit(1);
		}
		else
			waitpid(pid, NULL, 0);
		if (prev_pipe != -1)
			close(prev_pipe);
		if (i < msh->data->pipes)
		{
			close(pipefd[1]);
			prev_pipe = pipefd[0];
		}
		while (current_token && current_token->type != TKN_PIPE)
			current_token = current_token->next;
		if (current_token && current_token->type == TKN_PIPE)
			current_token = current_token->next;
		i++;
	}
	if (prev_pipe != -1)
		close(prev_pipe);
	return (0);
}

int execute(t_msh *msh)
{
	if (open_files(msh) != 0)
	{
		g_exit = 1;
		return (-1);
	}
	if (msh->data->pipes == 0) // Se identificado 1 pipe pelo menos, não entra aqui
		execute_one(msh, msh->envp);
	else
		execute_multi(msh);
	return (0);
}
