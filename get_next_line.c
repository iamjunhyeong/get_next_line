/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:58:03 by junhyeop          #+#    #+#             */
/*   Updated: 2023/11/17 20:23:50 by junhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*find_newline(char *line, t_gnl_list *tmp)
{
	sizt_t	i;
	int		found;

	found = 0;
	i = 0;
	while (tmp->backup[i] != 0)
	{
		while (tmp->backup[i++] == '\n')
		{
			found = 1;
			line = malloc(i);
			ft_strlcpy(line, tmp->backup, i);
			break ;
		}
	}
	if (found)
		ft_strlcpy(tmp->backup, tmp->backup + i, ft_strlen(tmp->backup) + 1);
	return (found);
}

char	*read_line(t_gnl_list *tmp, int fd)
{
	char	*buf;
	char	*pre;
	int		n;

	buf = (char *)malloc(sizeof(char), BUF_SIZE + 1);
	if (!buf)
		return (NULL);
	if (n = read(fd, tmp->backup, BUF_SIZE) < 0)
	{
		free(buf);
		return (NULL);
	}
	buf[n] = 0;
	pre = tmp->backup;
	tmp->backup = ft_strjoin(pre, buf);
	if (!tmp->backup)
	{
		free(pre);
		return (NULL);
	}
	free(pre);
	return (tmp->backup);
}

t_gnl_list	*ft_lstnew(int fd)
{
	t_gnl_list	*new;
	int			byte;

	new->fd = fd;
	new->next = NULL;
	new->backup = NULL;
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
		{
			if (!(tmp->next = ft_lstnew(fd)))
				return (NULL);
			return (tmp);
		}
		tmp = tmp->next;
	}
	if (!(head = ft_lstnew(fd)))
		return (NULL);
	return (head);
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*head;
	t_gnl_list			*tmp;
	t_str_list			*str;

	if (fd < 0 || BUF_SIZE <= 0)
		return (_ERROR);
	if (!head)
		head = find_fd(head, fd , NULL);
	if (!head)
		return (_ERROR);
	tmp = find_fd(head, fd, NULL);
	if (tmp->backup)
		load_backup(str, tmp);
	while (1)
	{
		if (str->s != -1)
			res = read_line(tmp, fd);
	
		
	}
}
