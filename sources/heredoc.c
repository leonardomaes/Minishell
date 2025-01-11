/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 00:09:07 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/01/11 01:11:31 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//checks if within a heredoc, there are variables to expand
int	has_expand(const char *line)
{
	int	i;

	i = -1;
	while (line[++i])
		if (line[i] == '$' && line[i + 1] != '\0')
			return (1);
	return (0); //no variable found
}

int	search_variable_end(const char *s, int i)
{
	while (s[++i])
	{
		if (s[i] == '?' && s[i - 1] == '$')
			return (i + 1);
		if (!ft_isalnum(s[i]))
			return (i);
	}
	return (i);
}

void	get_expand_variable(char *line, t_msh *msh, t_expand *exp)
{
	char	*key;
	char	*content;
	char	*tmp;

	exp->position = search_variable_end(line + exp->i + 1, -1);
	key = ft_substr(line, exp->i + 1, exp->position);
	exp->new = ft_substr(line, exp->start, exp->i);
	tmp = exp->end; 
	exp->end = ft_strjoin(exp->end, exp->new);
	free(tmp);
	if (line[exp->i + 1] != '?')
		content = get_env_var_value(msh->envp, key);
	else
		content = ft_itoa(g_exit);
	if (content)
	{
		tmp = exp->end; 
		exp->end = ft_strjoin(exp->end, content);
		free(tmp); 
	}
	exp->i += exp->position;
	exp->start = exp->i + 1;
	free(exp->new);
	if (line[exp->i + 1] == '?')
		free(content);
	free(key);
}

//processses input and replace the variables ($VAR)
char	*expand_line(char *line, t_msh *msh)
{
	t_expand	*exp;
	char		*tail;
	char		*result;

	exp = malloc(sizeof(t_expand));
	if (!exp)
		return (NULL);
	exp->i = 0;
	exp->start = 0;
	exp->end = ft_strdup("");
	while (line[exp->i])
	{
		if (line[exp->i] == '$' && line[exp->i + 1] != '\0')
			get_expand_variable(line, msh, exp);
		exp->i++;
	}
	tail = ft_substr(line, exp->start, exp->i - exp->start);
	exp->end = ft_strjoin(exp->end, tail);
	result = ft_strdup(exp->end);
	free(tail);
	free(exp->end);
	free(exp);
	free(line);
	return (result);
}

//functions that reads line until delimiter (end) is found, expands variables if delimiter is not quoted and stores the input in a file
void	handle_heredoc(char *delimiter, t_msh *msh)
{
	char	*line;
	int		fd;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
	set_signal(HEREDOC, msh);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file\n", 2);
			break ;
		}
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		if (has_expand(line))
			line = expand_line(line, msh);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	set_signal(SHELL_MODE, msh);
}