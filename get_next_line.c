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
		copy = ft_strndup(var.str, var.len);
		if (!copy)
			return (NULL);
		tmp->backup = ft_strndup(&var.str[var.len], BUFFER_SIZE);
		if (!tmp->backup)
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

	while (!found)
	{
		var.str = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!var.str)
			return (NULL);
		var.n = read(fd, var.str, BUFFER_SIZE);
		if (var.n <= 0)
		{
			free(var.str);
			tmp->eof = 1;
			break ;
		}
		line = read_line_ext(&var, line, tmp, &found);
		if (!line)
			return (NULL);
		// free(var.str);
	}
	return (line);
}

char	*read_line_ext(t_var *var, char *line, t_gnl_list *tmp, int *found)
{
	char	*lline;

	var->str[var->n] = '\0';
	var->len = find_newline(var->str, tmp);
	if (line == NULL)
		line = ft_strndup("", 0);
	if (line == NULL)
	{
		free(var->str);
		return (NULL);
	}
	if (var->len)
	{
		if (var->str[var->len])
		{
			tmp->backup = ft_strndup(&var->str[var->len], BUFFER_SIZE);
			if (tmp->backup == NULL)
			{
				free(var->str);
				return (NULL);
			}
		}
		lline = ft_strjoin(line, var->str, var->len);
		*found = 1;
		return (lline);
	}
	lline = ft_strjoin(line, var->str, BUFFER_SIZE);
	if (!lline)
		return (NULL);
	return (lline);
}

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
	t_gnl_var			var;

	var = (struct s_gnl_var){0, NULL, NULL};
	if (read(fd, NULL, 0) == -1 || BUFFER_SIZE <= 0)
	{
		var.tmp = find_fd(&head, fd, NULL);
		if (!var.tmp)
			return (NULL);
		lst_delone(var.tmp, &head, NULL);
		return (NULL);
	}
	var.tmp = find_fd(&head, fd, NULL);
	if (!var.tmp)
		return (NULL);
	if (var.tmp->backup)
	{
		var.line = load_backup(var.tmp, &var.found);
		if (var.line == NULL)
			return (NULL);
	}
	var.line = read_line(var.tmp, var.line, fd, var.found);
	if (var.line == NULL || var.tmp->eof == 1)
		lst_delone(var.tmp, &head, NULL);
	return (var.line);
}
