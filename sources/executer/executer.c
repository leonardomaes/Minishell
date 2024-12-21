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
		execute_echo(msh->data->args);
	/* else if(msh->data->tokens->type == BLT_CD)
		execute_cd();
	else if(msh->data->tokens->type == BLT_PWD)
		execute_pwd();
	else if(msh->data->tokens->type == BLT_EXPORT)
		execute_export();
	else if(msh->data->tokens->type == BLT_UNSET)
		execute_unset();
	else if(msh->data->tokens->type == BLT_ENV)
		execute_env();
	else if(msh->data->tokens->type == BLT_EXIT)
		execute_exit(); */
	return (0);
}

void child_process(t_msh *msh)
{
	char *comm;

	comm = ft_get_command(msh->data->tokens->name, msh->cmd_paths);
	if (!comm)
		printf("bash: %s: command not found", msh->data->tokens->name);
	execve(comm, msh->data->args, msh->envp);
	free(comm);
}

int execute(t_msh *msh)
{
	pid_t pid;

	if (msh->data->tokens->type >= 101 && msh->data->tokens->type >= 107)
		exec_builtin(msh);
	else
	{
		pid = fork();
		if (pid < 0)
			return (-1);
		else if (pid == 0)
			child_process(msh);
	}
	waitpid(pid, NULL, 0);
	return (0);
}

/* int execute(t_msh *msh)
{
	if (msh->data->tokens->type >= 101 && msh->data->tokens->type <= 107)
		exec_builtin(msh);
	return (0);
} */