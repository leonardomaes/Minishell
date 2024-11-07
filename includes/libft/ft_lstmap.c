/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 13:11:31 by lmaes             #+#    #+#             */
/*   Updated: 2024/05/03 14:27:51 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*lst_new(void *contend)
{
	t_list	*x;

	x = (t_list *)malloc(sizeof(t_list));
	if (!x)
		return (NULL);
	x->content = (void *)contend;
	x->next = NULL;
	return (x);
}

static void	lst_clear(t_list **lst, void (*del)(void *))
{
	if (!lst || !del || !(*lst))
		return ;
	lst_clear(&(*lst)->next, del);
	(del)((*lst)->content);
	free(*lst);
	*lst = NULL;
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*temp;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = lst_new(f(lst->content));
	if (!new_lst)
		return (NULL);
	temp = new_lst;
	lst = lst->next;
	while (lst != NULL)
	{
		new_lst->next = lst_new(f(lst->content));
		if (!new_lst->content)
		{
			lst_clear(&new_lst, del);
			return (NULL);
		}
		new_lst = new_lst->next;
		lst = lst->next;
	}
	return (temp);
}
