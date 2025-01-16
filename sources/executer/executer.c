/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:20:00 by lmaes             #+#    #+#             */
/*   Updated: 2025/01/16 00:22:15 by rda-cunh         ###   ########.fr       */
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
	/* ft_free_all(msh);
	exit(127); */
	return (0);
}

//sets input redirection for heredoc (sdin reads from file)
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
	pid_t pid;

	//introduced a first pass trought all tokens to handle heredocs (must he taken care first to input redirection)
	setup_heredocs(msh->data->tokens, msh);

	//skip if token is a redirection/heredoc
	if (msh->data->tokens->type == TKN_HEREDOC)
		return (0);

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
			return (-1);
		waitpid(pid, NULL, 0);
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
	ft_print_array(msh->envp);
	setup_heredocs(msh->data->tokens, msh); //setups all heredocs prior to execution
	while (i <= msh->data->pipes)
	{
		if (i < msh->data->pipes && pipe(pipefd) == -1) // Se não for ultimo e der erro no pipe
		{
			perror("pipe:");
			exit(1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork:");
			exit(1);
		}
		else if (pid == 0)
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
			handle_redirections(current_token); //calls redirection function to deal with heredoc (and probably other redirections -- to check with Leo)
			msh->data->tokens = current_token;
			execute_one(msh, msh->envp);
			ft_free_all(msh);
			exit(1);
		}
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
	ft_print_array(msh->envp);
	if (prev_pipe != -1)
		close(prev_pipe);
	while (i-- > 0)
		waitpid(-1, NULL, 0);
	ft_free_all(msh);
	exit(1);
	return (0);
}
/* int execute(t_msh *msh)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		if (msh->data->pipes == 0) // Se identificado 1 pipe pelo menos, não entra aqui
			execute_one(msh, msh->envp);
		else
			execute_multi(msh);
	}
	else if(pid < 0)
		return (-1);
	//ft_print_array(msh->envp);
	waitpid(pid, NULL, 0);
	return (0);
} */

int execute(t_msh *msh)
{
	if (msh->data->pipes == 0) // Se identificado 1 pipe pelo menos, não entra aqui
		execute_one(msh, msh->envp);
	else
		execute_multi(msh);
	//ft_print_array(msh->envp);
	return (0);
}
