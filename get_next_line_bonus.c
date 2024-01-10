/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:28:01 by junhyeop          #+#    #+#             */
/*   Updated: 2024/01/10 19:28:01 by junhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*load_backup(t_var *var, char **backup, char *result, char *temp)
{
	var->len_nl = find_newline(*backup);
	temp = *backup;
	if (var->len_nl)
	{
		var->found = 1;
		if (temp[var->len_nl])
		{
			*backup = ft_strndup(&temp[var->len_nl], BUFFER_SIZE);
			if (!*backup)
			{
				free(temp);
				return (NULL);
			}
		}
		else
			*backup = NULL;
		result = ft_strndup(temp, var->len_nl);
	}
	else
	{
		result = ft_strndup(temp, BUFFER_SIZE);
		*backup = NULL;
	}
	free(temp);
	return (result);
}

char	*read_line(t_var *var, int fd)
{
	var->buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!var->buf)
	{
		free(var->line);
		return (NULL);
	}
	while (!var->found)
	{
		var->len_read = read(fd, var->buf, BUFFER_SIZE);
		if (var->len_read < 0)
		{
			free(var->line);
			var->line = NULL;
		}
		if (var->len_read <= 0)
			break ;
		if (!read_line_ext(var))
		{
			free(var->line);
			var->line = NULL;
			break ;
		}
	}
	free(var->buf);
	return (var->line);
}

char	*read_line_ext(t_var *var)
{
	char	*temp;

	var->buf[var->len_read] = '\0';
	var->len_nl = find_newline(var->buf);
	if (var->len_nl)
	{
		var->found = 1;
		if (var->buf[var->len_nl])
		{
			var->tmp->backup = ft_strndup(
					&var->buf[var->len_nl], var->len_read - var->len_nl);
			if (!var->tmp->backup)
				return (NULL);
		}
	}
	temp = var->line;
	if (var->len_nl)
		var->line = ft_strjoin(temp, var->buf, var->len_nl);
	else
		var->line = ft_strjoin(temp, var->buf, var->len_read);
	free(temp);
	return (var->line);
}

t_gnl_list	*find_fd(t_gnl_list **head, int fd, t_gnl_list *tmp)
{
	if (*head == NULL)
	{
		*head = ft_lstnew(fd);
		if (*head == NULL)
			return (NULL);
	}
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
	t_var				var;

	var = (t_var){0, NULL, NULL, 0, 0, NULL};
	var.tmp = find_fd(&head, fd, NULL);
	if (!var.tmp)
		return (NULL);
	if (read(fd, NULL, 0) == -1 || BUFFER_SIZE <= 0)
	{
		lst_delone(var.tmp, &head, NULL);
		return (NULL);
	}
	if (var.tmp->backup)
	{
		var.line = load_backup(&var, &var.tmp->backup, NULL, NULL);
		if (var.line == NULL)
		{
			lst_delone(var.tmp, &head, NULL);
			return (NULL);
		}
	}
	var.line = read_line(&var, fd);
	if (!var.line)
		lst_delone(var.tmp, &head, NULL);
	return (var.line);
}
