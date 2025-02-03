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

int exec_builtin(t_msh *msh, t_tokens *tokens)
{
	if (msh->data->tokens->type == BLT_ECHO)
		return (execute_echo(tokens->args));
	else if(msh->data->tokens->type == BLT_CD)
		return (execute_cd(msh, msh->data->tokens->args));
	else if(msh->data->tokens->type == BLT_PWD)
		return (execute_pwd());
	else if(msh->data->tokens->type == BLT_EXPORT)
		return (execute_export(msh, msh->data->tokens->args));
	else if(msh->data->tokens->type == BLT_UNSET)
		return (execute_unset(msh, msh->data->tokens->args));
	else if(msh->data->tokens->type == BLT_ENV)
		return (execute_env(msh->envp));
	else if(msh->data->tokens->type == BLT_EXIT)
		return (execute_exit(msh, msh->data->tokens->args));
	return (1);
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
			handle_heredoc(current, msh);
		current = current->next;
	}
}

int execute_one(t_msh *msh, char **envp)
{
	char 	*comm;
	char	 *cwd;
	int		status;
	pid_t 	pid;

	//introduced a first pass trought all tokens to handle heredocs (must he taken care first to input redirection)
	setup_heredocs(msh->data->tokens, msh);
	//for TKN_HEREDOC alone, just skip
	//g_exit = 1;
	if (msh->data->tokens->type == TKN_HEREDOC)
		return (0);
	if (open_files(msh, msh->data->tokens) != 0)
	{
		g_exit = 1;
		return (-1);
	}
	if (msh->data->infile > 0)	// Ta fazendo exit quando tem infile
		dup2(msh->data->infile, STDIN_FILENO);
	if (msh->data->outfile > 0)
		dup2(msh->data->outfile, STDOUT_FILENO);
	if (msh->data->tokens->type >= 101 && msh->data->tokens->type <= 107)
		g_exit = exec_builtin(msh, msh->data->tokens);
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
				// perror("bash: ");
				//printf("bash: %s: command not found\n", msh->data->tokens->name);
				ft_putstr_fd(" bash: ", 2);
				ft_putstr_fd(msh->data->tokens->name, 2);
				ft_putstr_fd(" command not found\n", 2);
				//free(cwd);
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
				exit(2);
			}
			free(comm);
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WEXITSTATUS(status))
				g_exit = WEXITSTATUS(status);
		}
	}
	return (0);
}

int execute_cmd(t_msh *msh, t_tokens *tokens, char **envp)
{
	char *comm;
	char *cwd;
	int status;

	cwd = NULL;
	if (tokens->type >= 101 && tokens->type <= 107)
	{
		status = exec_builtin(msh, tokens);
		ft_free_all(msh);
		exit(status);
	}
	else
	{
		if (tokens->type == TKN_BCMD && tokens->name[0] == '.')
		{
			cwd = getcwd(NULL, 0);
			comm = ft_strjoin(cwd, tokens->name + 1);
			free(cwd);
		}
		else
			comm = ft_get_command(tokens->name, msh->cmd_paths);
		if (!comm)
		{
			//printf("bash: %s: command not found\n", tokens->name);
			ft_putstr_fd(" bash: ", 2);
			ft_putstr_fd(msh->data->tokens->name, 2);
			ft_putstr_fd(" command not found\n", 2);
			ft_free_all(msh);
			exit(127);
		}
		if (execve(comm, tokens->args, envp) == -1)
		{
			perror("execve:");
			free(comm);
			ft_free_all(msh);
			exit(1);
		}
		free(comm);
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

	// Abre arquivos apenas no primeiro comando
	if (open_files(msh, current_token) != 0)
	{
		g_exit = 1;
		return (-1);
	}

	while (i <= msh->data->pipes)
	{
		if (i < msh->data->pipes && pipe(pipefd) == -1) // Criando pipe para o próximo comando
		{
			perror("pipe: ");
			exit(1);
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork: ");
			exit(1);
		}
		else if (pid == 0) // Processo filho
		{
			// 🔹 Redirecionamento de Entrada (Se houver infile ou pipe anterior)
			if (prev_pipe != -1) 
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			else if (msh->data->infile > 0) // Se houver infile aberto
			{
				dup2(msh->data->infile, STDIN_FILENO);
				close(msh->data->infile);
			}

			// 🔹 Redirecionamento de Saída (Se houver outfile ou próximo comando no pipeline)
			if (i < msh->data->pipes && msh->data->outfile == -1) 
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				close(pipefd[0]);
			}
			else if (msh->data->outfile > 0) // Se houver outfile aberto
			{
				dup2(msh->data->outfile, STDOUT_FILENO);
				close(msh->data->outfile);
			}

			// Executa o comando
			execute_cmd(msh, current_token, msh->envp);
			exit(1);
		}

		// Processo pai
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

	// 🔹 Restaurar entrada e saída padrão após execução dos comandos
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

	return (0);
}


int execute(t_msh *msh)
{
	int status;

	status = 1;
	/* if (open_files(msh) != 0)
	{
		g_exit = 1;
		return (-1);
	}
	if (msh->data->infile > 0)	// Ta fazendo exit quando tem infile
		dup2(msh->data->infile, STDIN_FILENO);
	if (msh->data->outfile > 0)
		dup2(msh->data->outfile, STDOUT_FILENO); */
	if (msh->data->pipes == 0)
		status = execute_one(msh, msh->envp);
	else
		status = execute_multi(msh);
	close_files(msh);
	return (status);
}
