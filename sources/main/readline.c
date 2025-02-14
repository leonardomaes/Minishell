/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/14 00:09:17 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_prompt(void)
{
	char	*line;

	line = readline("\001\033[1;36m\002cmd>\001\033[0m\002");
	if (!line)
		return (NULL);
	else if (*line != '\0')
		add_history(line);
	return (line);
}

int	get_redir(t_tokens *temp)
{
	if (temp->type == TKN_APPEND || temp->type == TKN_INREDIR
		|| temp->type == TKN_OUTREDIR || temp->type == TKN_HEREDOC)
		return (1);
	return (0);
}

int	ft_init_data(char *line, t_msh *msh)
{
	msh->data->cmd_paths = NULL;
	msh->data->cmd_paths = ft_split(ft_get_path((*msh).envp), ':');
	msh->data->args = ft_split_args(msh, line);
	if (!msh->data->args || !msh->data->args[0]
		|| (*msh->data->args[0] == '\0' && !msh->data->args[1]))
	{
		if (msh->data->args)
			free_array(msh->data->args, ft_countargs(msh->data->args));
		if (msh->data->cmd_paths)
			free_array(msh->data->cmd_paths,
				ft_countargs(msh->data->cmd_paths));
		return (1);
	}
	msh->data->argc = ft_countargs(msh->data->args);
	msh->data->pipes = 0;
	msh->data->infile = -2;
	msh->data->outfile = -2;
	msh->data->stdin_backup = -2;
	msh->data->stdout_backup = -2;
	return (0);
}

static int	handle_empty_line(t_msh *msh, char *line)
{
	if (*line == '\0')
	{
		free(msh->data);
		msh->data = NULL;
		free(line);
		return (1);
	}
	return (0);
}

int	ft_readline(t_msh *msh)
{
	char	*line;
	int		i;

	i = 0;
	msh->data = malloc(sizeof(t_data));
	if (!msh->data)
		return (1);
	line = ft_prompt();
	if (!line)
	{
		free(msh->data);
		msh->data = NULL;
		return (1);
	}
	if (handle_empty_line(msh, line))
		return (0);
	if (ft_init_data(line, msh) != 0)
		return (free(msh->data), free(line), 1);
	split_tokens(msh, &msh->data->tokens, NULL, i);
	if (syntax_check(msh, msh->data) != 0)
		return (ft_free_data(msh), free(line), 1);
	//ft_print_tokens(msh->data->tokens);
	ft_get_args(msh);
	return (free(line), 0);
}
