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

int	find_newline(char *str, t_gnl_list *tmp)
{
	t_var	var;
	int		len;

	len = 0;
	var = (struct s_var){0, 0, NULL};
	while (str[var.n] != 0)
	{
		if (str[var.n] == '\n')
		{
			if (tmp->backup)
			{
				free(tmp->backup);
				tmp->backup = NULL;
			}
			// if (str[var.n + 1] != 0)
			// 	tmp->backup = ft_strndup(&str[var.n + 1], BUFFER_SIZE);
			return (var.n + 1);
		}
		var.n++;
	}
	return (0);
}

char	*load_backup(t_gnl_list *tmp, int *found)
{
	t_var	var;
	char	*copy;

	var = (struct s_var){0, 0, NULL};
	var.str = ft_strndup(tmp->backup, BUFFER_SIZE);
	if (!var.str)
		return (NULL);
	var.len = find_newline(var.str, tmp);
	if (var.len && var.str[var.len] != 0)
	{
		*found = 1;
		copy = ft_strndup(var.str, var.len + 1);
		tmp->backup = ft_strndup(&var.str[var.len], BUFFER_SIZE);
		if (!copy || !tmp->backup)
			return (NULL);
		free(var.str);
		return (copy);
	}
	copy = ft_strndup(var.str, BUFFER_SIZE);
	if (!copy)
		return (NULL);
	free(var.str);
	return (copy);
}

char	*read_line(t_gnl_list *tmp, char *line, int fd, int found)
{
	t_var	var;

	if (tmp->backup)
	{
		line = load_backup(tmp, &found);
		if (line == NULL)
			return (NULL);
	}
	while (!found)
	{
		var.str = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!var.str)
			return (NULL);
		var.n = read(fd, var.str, BUFFER_SIZE);
		if (var.n <= 0)
		{
			free(var.str);
			break ;
		}
		var.str[var.n] = '\0';
		var.len = find_newline(var.str, tmp);
		if (line == NULL)
			line = ft_strndup("", 0);
		if (line == NULL)
		{
			free(var.str);
			return (NULL);
		}
		if (var.len) // newline exists
		{
			if (var.str[var.len])
			{
				tmp->backup = ft_strndup(&var.str[var.len], BUFFER_SIZE);
				if (tmp->backup == NULL)
				{
					free(var.str);
					return (NULL);
				}
			}
			line = ft_strjoin(line, var.str, var.len);
			free(var.str);
			if (!line)
				return (NULL);
			return (line);
		}
		line = ft_strjoin(line, var.str, BUFFER_SIZE);
		free(var.str);
		if (!line)
			return (NULL);
	}
	tmp->eof = 1;
	return (line);
}

// char	*newline_exists(int len, char **line, char **str);
// {

// }



t_gnl_list	*find_fd(t_gnl_list **head, int fd, t_gnl_list *tmp)
{
	if (*head == NULL)
		*head = ft_lstnew(fd);
	if (*head == NULL)
		return (NULL);
	tmp = *head;
	while (tmp)
	{
		if (tmp->fd == fd)
			return (tmp);
		if (tmp->next == NULL)
		{
			tmp->next = ft_lstnew(fd);
			if (!tmp->next)
				return (NULL);
			return (tmp->next);
		}
		tmp = tmp->next;
	}
	return (*head);
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*head;
	t_gnl_list			*tmp;
	char				*str;
	
	if (read(fd, NULL, 0) == -1 || BUFFER_SIZE <= 0)
		return (NULL);
	tmp = find_fd(&head, fd, NULL);
	if (!tmp)
		return (NULL);
	str = read_line(tmp, NULL, fd, 0);
	if (!str || tmp->eof)
	{
		lst_delone(tmp, &head, NULL);
		return (str);
	}
	return (str);
}