/* ************************************************************************** */*                                                                            */
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

char	*find_newline(char *line, char *str)
{
	size_t	i;
	char	*s;
	int		found;

	found = 0;
	i = 0;
	str = NULL;
	while (line[i] != 0)
	{
		while (line[i++] == '\n')
		{
			found = 1;
			// s = (char *)malloc(sizeof(char) * i + 1);
			// ft_strlcpy(s, line, i);
			// free(line);
			break ;
		}
	}
	// if (found)
	// 	ft_strjoin(str, s);
	return (found);
}

char	*read_line(t_gnl_list *tmp, char *str, int fd)
{
	int			n;
	char		*line;

	line = (char *)malloc(sizeof(char) * BUF_SIZE + 1);
	if (!line)
		return (NULL);
	if (n = read(fd, line, BUF_SIZE) <= 0)
	{
		free(line);
		tmp->eof = -1;
		return (NULL);
	}
	line[n] = '\0';
	if (find_newline(line))
	{
		str = ft_strjoin(str, line);
		return (1);
	}
	str = ft_strjoin(str, line);
	return (0);
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

void	lst_delone(t_gnl_list *remove, t_gnl_list *haed)
{
	t_gnl_list	*tmp;

	tmp = head;
	while (!tmp)
	{
		if (tmp->next == remove)
		{
			tmp->next = remove->next;
			free(remove->backup);
			free(remove);
		}
		tmp = tmp->next;
	}
}

t_gnl_list	*find_fd(t_gnl_list *tmp, int fd, t_gnl_list *head)
{
	head = tmp;
	while (tmp)
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
	char				*str;

	if (fd < 0 || BUF_SIZE <= 0)
		return (_ERROR);
	if (!head)
		head = find_fd(head, fd , NULL);
	if (!head)
		return (_ERROR);
	tmp = find_fd(head, fd, NULL);
	str = NULL;
	if (tmp->backup)
		str = load_backup(tmp);
	while (1)
	{
		if (read_line(tmp, str, fd))
			return (str);
		if (tmp->eof)
		{
			lst_delone(tmp);
			return (str);
		}
	}
}
