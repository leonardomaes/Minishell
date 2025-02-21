/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2025/02/12 13:40:47 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	syntax_redirs(t_msh *msh, t_tokens *tokens)
{
	t_tokens	*temp;

	temp = tokens;
	while (temp)
	{
		if (get_redir(temp) == 1)
			break ;
		temp = temp->next;
	}
	if (!temp)
		return (0);
	if (temp->next && temp->next->type == TKN_SPACE)
		temp = temp->next;
	if (temp->next == NULL)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		return (g_exit = 2, 1);
	}
	if (get_delimiter(msh, temp->next->name) != 0 || temp->next->type == TKN_HEREDOC)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(temp->next->name, 2);
		return (ft_putstr_fd("'\n", 2), g_exit = 2, 1);
	}
	return (0);
}

int	syntax_pipes(t_tokens *tokens)
{
	t_tokens	*temp;

	temp = tokens;
	while (temp)
	{
		if (temp->type == TKN_PIPE && (temp->next == NULL
				|| temp->next->type == TKN_PIPE))
		{
			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
			ft_putstr_fd(temp->name, 2);
			ft_putstr_fd("'\n", 2);
			return (1);
		}
		if (temp->type == TKN_PIPE && temp->next == NULL)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
			ft_putstr_fd(temp->name, 2);
			ft_putstr_fd("'\n", 2);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

static int	check_matching_quote(char *str, char quote_type)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i + 1] == quote_type)
			return (0);
		else if (str[i + 1] == '\0')
			return (1);
		i++;
	}
	return (1);
}

int	syntax_quotes(t_tokens *tokens)
{
	t_tokens	*temp;

	temp = tokens;
	while (temp)
	{
		if (temp->type == DBL_QUOTES && check_matching_quote(temp->name, '"'))
			return (1);
		if (temp->type == SNG_QUOTES && check_matching_quote(temp->name, '\''))
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	syntax_check(t_msh *msh, t_data *data)
{
	if (data->tokens->type == TKN_PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(data->tokens->name, 2);
		ft_putstr_fd("'\n", 2);
		return (g_exit = 2, 1);
	}
	if (syntax_quotes(data->tokens))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `open quote'\n",
			2);
		g_exit = 2;
		return (1);
	}
	if (syntax_pipes(data->tokens) != 0)
		return (g_exit = 2, 1);
	if (syntax_redirs(msh, data->tokens) != 0)
		return (g_exit = 2, 1);
	return (0);
}
