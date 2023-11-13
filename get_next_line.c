/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:58:03 by junhyeop          #+#    #+#             */
/*   Updated: 2023/11/13 22:05:23 by junhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_line(t_gnl_list **tmp, int fd)
{
	char	*buffer;

	buffer = ft_calloc(BUF_SIZE + 1, sizeof(char));
	
}

t_gnl_list	*ft_lstnew(int fd)
{
	t_gnl_list	*new;
	int			byte;

	new->fd = fd;
	new->next = NULL;
	new->backup = NULL;
	new->idx = 0;
	return (new);
}

t_gnl_list	*find_fd(t_gnl_list *tmp, int fd, t_gnl_list *head)
{
	head = tmp;
	while (1)
	{
		if (tmp->fd == fd)
			return (tmp);
		if (tmp->next == NULL)
			break ;
		tmp = tmp->next;
	}
	tmp->next = ft_lstnew(fd);
	if (tmp->next == NULL);
		return (NULL);
	return (head);
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*head;
	t_gnl_list			*tmp;
	char				*res;

	if (fd < 0 || BUF_SIZE <= 0)
		return (_ERROR);
	if (!head)
		head = find_fd(head, fd , NULL);
	if (!head)
		return (_ERROR);
	tmp = find_fd(head, fd, NULL);
	res = NULL;
	if (tmp->backup)
		res = is_nl_backup(&tmp->backup, ft_len_free(tmp->backup, 0), tmp);
	while (1)
	{
		if(!res)
			res = read_line(&tmp, fd);
	}
}
