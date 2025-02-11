/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 00:09:07 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/02/10 02:54:01 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//checks if within a heredoc, there are variables to expand

int calculate_expanded_length(t_msh *msh, const char *line)
{
	int			i;
	int			new_len;
	const char	*var_start;
	const char	*var_end;
	const char	*var_value;
	char		*var_name;

	i = 0;
	new_len = 0;
	while (line[i])
	{
		if (line[i] == '$' && isalnum(line[i + 1]))
		{
			var_start = &line[i + 1];
			var_end = var_start;
			while (*var_end && (isalnum(*var_end) || *var_end == '_'))
				var_end++;
			var_name = ft_strndup(var_start, var_end - var_start);
			var_value = get_env_var_value(msh->envp, var_name);
			free(var_name);
 
			if (var_value)
				new_len += ft_strlen(var_value);
			i += (var_end - var_start) + 1;
		}
		else
		{
			new_len++;
			i++;
		}
	}
	return new_len;
}

char *expand_env_variables(t_msh *msh, const char *line)
{
	int 		i;
	int			j;
	int 		new_len;
	const char	*var_start;
	const char	*var_end;
	const char	*var_value;
	char		*var_name;
	char		*result;

	i = 0;
	j = 0;
	new_len = calculate_expanded_length(msh, line);
	result = malloc(new_len + 1);
	if (!result)
		return NULL;
	while (line[i])
	{
		if (line[i] == '$' && isalnum(line[i + 1]))
		{
			var_start = &line[i + 1];
			var_end = var_start;
			while (*var_end && (isalnum(*var_end) || *var_end == '_'))
				var_end++;
			var_name = ft_strndup(var_start, var_end - var_start);
			var_value = get_env_var_value(msh->envp, var_name);
			free(var_name);
			if (var_value)
			{
				while (*var_value)
					result[j++] = *var_value++;
			}
			i += (var_end - var_start) + 1;
		}
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return result;
}

//functions that reads line until delimiter (end) is found, expands variables if delimiter is not quoted and stores the input in a file
void	handle_heredoc(t_tokens *token, t_msh *msh)
{
	char	*line;
	int		fd;
	int 	status;
	pid_t	pid;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	{
		set_signal(HEREDOC, msh);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				close (fd);
				ft_putstr_fd("warning: here-document delimited by end-of-file\n", 2);
				ft_free_all(msh); 
				exit(0);
			}
			if (!ft_strcmp(line, token->args[0]))
			{
				free(line);
				break ;
			}
			if (token->next->type != SNG_QUOTES && token->next->type != DBL_QUOTES)
				line = expand_env_variables(msh, line);
			ft_putendl_fd(line, fd);
			free(line);
		}
		close(fd);
		ft_free_all(msh);
		exit(0);
	}
	else //parent process
	{
		set_signal(HEREDOC_PAUSE, msh);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) || WEXITSTATUS(status)) //checks if the child process was terminated by a signal (SIGINT / Ctrl+C)
		{
			g_exit = 130;
			close(fd);
			return ;
		}
		close(fd);
		set_signal(SHELL_MODE, msh);
	}
}
