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

void	handle_redirections(t_tokens *token) //added this to handle heredoc (must evaluate how to melt with redirections code from Leonardo)
{
	int	fd;
	
	while (token)
	{
		if (token->type == TKN_HEREDOC)
		{
			fd = open(".heredoc_tmp", O_RDONLY); //opens heredoc file
			if (fd < 0)
			{
				perror("open heredoc_tmp");
				exit(1);
			}
			if (dup2(fd, STDIN_FILENO) == -1) //redirects stdin to read from the heredoc file
			{
				perror("dup2");
				exit(1);
			}
			close(fd);
			unlink(".heredoc_tmp"); //remove a link to a file in filesystem
			break ; //only use the last heredoc (multi heredocs)
		}
		token = token->next;
	}
}

//prepare all heredocs before pipeline execution (must be called before creating any pipes)
void	setup_heredocs(t_tokens *tokens, t_msh *msh)
{
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TKN_HEREDOC)
			handle_heredoc(current->args[0], msh);
		current = current->next;
	}
}

int execute_one(t_msh *msh, char **envp)
{
	char *comm;
	char *cwd;
	pid_t pid;

	//introduced a first pass trought all tokens to handle heredocs (must he taken care first to input redirection)
	setup_heredocs(msh->data->tokens, msh);
	//for TKN_HEREDOC alone, just skip
	if (msh->data->tokens->type == TKN_HEREDOC)
		return (0);
	if (msh->data->tokens->type >= 101 && msh->data->tokens->type <= 107)
		exec_builtin(msh);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			handle_redirections(msh->data->tokens);
			if (msh->data->tokens->type == TKN_BCMD && msh->data->tokens->name[0] == '.')
			{
				cwd = getcwd(NULL, 0);
				comm = ft_strjoin(cwd, msh->data->tokens->name + 1);
			}
			else
				comm = ft_get_command(msh->data->tokens->name, msh->cmd_paths);
			if (!comm)
			{
				printf("bash: %s: command not found\n", msh->data->tokens->name);
				free(cwd);
				free(comm);
				ft_free_all(msh);
				exit(127);
			}
			//printf("%s\n", comm); // Apagar
			if (execve(comm, msh->data->tokens->args, envp) == -1)
			{
				perror("execve:");
				free(cwd);
				free(comm);
				ft_free_all(msh);
				exit(1);
			}
			free(comm);
		}
		else
			waitpid(pid, NULL, 0);
	}
	return (0);
}

int execute_cmd(t_msh *msh, char **envp)
{
	char *comm;
	char *cwd;

	if (msh->data->tokens->type >= 101 && msh->data->tokens->type <= 107)
		exec_builtin(msh);
	else
	{
		if (msh->data->tokens->type == TKN_BCMD && msh->data->tokens->name[0] == '.')
		{
			cwd = getcwd(NULL, 0);
			comm = ft_strjoin(cwd, msh->data->tokens->name + 1);
		}
		else
			comm = ft_get_command(msh->data->tokens->name, msh->cmd_paths);
		if (!comm)
		{
			printf("bash: %s: command not found\n", msh->data->tokens->name);
			free(cwd);
			free(comm);
			ft_free_all(msh);
			exit(127);
		}
		if (execve(comm, msh->data->tokens->args, envp) == -1)
		{
			perror("execve:");
			free(cwd);
			free(comm);
			ft_free_all(msh);
			exit(1);
		}
		free(comm);
	}
	return (0);
}

int	execute_multi(t_msh *msh)		// Problema esta aqui quando faz comando com pipe, entra em fork duplo e causa leaks
{
	int		pipefd[2];
	int		prev_pipe;
	int		i;
	pid_t 	pid;
	t_tokens *current_token;

	i = 0;
	prev_pipe = -1;
	current_token = msh->data->tokens;
	setup_heredocs(msh->data->tokens, msh);
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
			handle_redirections(current_token);
			msh->data->tokens = current_token;
			execute_cmd(msh, msh->envp);
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
	while (i-- > 0)
		waitpid(-1, NULL, 0);
	return (0);
}

int execute(t_msh *msh)
{
	if (open_files(msh) != 0)
	{
		g_exit = 1;
		return (-1);
	}
	if (msh->data->infile > 0)	// Ta fazendo exit quando tem infile
		dup2(msh->data->infile, STDIN_FILENO);
	if (msh->data->outfile > 0)
		dup2(msh->data->outfile, STDOUT_FILENO);
	if (msh->data->pipes == 0)
		execute_one(msh, msh->envp);
	else
		execute_multi(msh);
	if (msh->data->infile > 0)
	{
		close(msh->data->infile);
		msh->data->infile = -1;
		dup2(msh->data->stdin_backup, STDIN_FILENO);
		close(msh->data->stdin_backup);
	}
	if (msh->data->outfile > 0)
	{
		close(msh->data->outfile);
		msh->data->outfile = -1;
		dup2(msh->data->stdout_backup, STDOUT_FILENO);
		close(msh->data->stdout_backup);
	}
	return (0);
}
