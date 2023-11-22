/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:25:24 by junhyeop          #+#    #+#             */
/*   Updated: 2023/11/22 16:25:24 by junhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	find_newline(char *line, t_gnl_list *tmp, int isload)
{
	size_t	i;
	int		found;

	found = 0;
	i = 0;
	while (line[i] != 0)
	{
		while (line[i++] == '\n')
		{
			found = 1;
			if (isload)
			{
				free(tmp->backup);
				tmp->backup = NULL;
				return (1);
			}
			tmp->backup = ft_strndup(&line[i], BUFFER_SIZE);
			return (i);
		}
	}
	return (0);
}

int	read_line(t_gnl_list *tmp, char **str, int fd)
{
	int			n;
	int			len;
	char		*line;

	line = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!line)
		return (0);
	if ((n = read(fd, line, BUFFER_SIZE)) <= 0)
	{
		free(line);
		tmp->eof = 1;
		return (0);
	}
	line[n] = '\0';
	len = find_newline(line, tmp, 0);
	if (len)
	{
		if (!*str)
			*str = ft_strndup(line, len);
		else
			*str = ft_strjoin(*str, line);
		return (1);
	}
	*str = ft_strjoin(*str, line);
	return (0);
}

t_gnl_list	*ft_lstnew(int fd)
{
	t_gnl_list	*new;

	new = (t_gnl_list*)malloc(sizeof(t_gnl_list));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->eof = 0;
	new->next = NULL;
	new->backup = NULL;
	return (new);
}

void	lst_delone(t_gnl_list *remove, t_gnl_list *head)
{
	t_gnl_list	*tmp;

	tmp = head;
	if (remove == head)
	{
		remove = head;
		head = head->next;
		if (remove->backup)
			free(remove->backup);
		free(remove);
		return ;
	}
	while (!tmp)
	{
		if (tmp->next == remove)
		{
			tmp->next = remove->next;
			if (remove->backup)
				free(remove->backup);
			free(remove);
			return ;
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
			return (tmp->next);
		}
		tmp = tmp->next;
	}
	if (!(head = ft_lstnew(fd)))
		return (NULL);
	return (head);
}

char	*load_backup(t_gnl_list *tmp)
{
	char	*str;
	char	*copy;

	str = tmp->backup;
	if (find_newline(str, tmp, 0))
	{
		copy = ft_strndup(str, BUFFER_SIZE);
		free(str);
		return (copy);
	}
	copy = ft_strndup(str, BUFFER_SIZE);
	free(str);
	tmp->backup = NULL;
	return (copy);
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*head;
	t_gnl_list			*tmp;
	char				*str;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!head)
		head = find_fd(head, fd , NULL);
	if (!head)
		return (NULL);
	tmp = find_fd(head, fd, NULL);
	if (!tmp)
		return (NULL);
	str = NULL;
	if (tmp->backup)
	{
		str = load_backup(tmp);
	}
	while (1)
	{
		if (!str)
			read_line(tmp, &str, fd);
		if (tmp->eof || str)
		{
			if (tmp->eof)
				lst_delone(tmp, head);
			return (str);
		}
	}
}