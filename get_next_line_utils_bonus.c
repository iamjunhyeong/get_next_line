/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:03:07 by junhyeop          #+#    #+#             */
/*   Updated: 2023/11/17 19:55:55 by junhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strndup(const char *str, size_t n)
{
	size_t	len;
	size_t	i;
	char	*copy;

	len = 0;
	i = 0;
	while (len < n && str[len])
		len++;
	copy = malloc(len + 1);
	if (!copy)
		return (NULL);
	while (len - i > 0 && str[i])
	{
		copy[i] = str[i];
		i++;
	}
	copy[len] = '\0';
	return (copy);
}

char	*ft_strjoin(char *s1, char *s2, int n)
{
	t_strjoin_var	var;

	var = (struct s_strjoin_var){0, 0, 0, 0, NULL};
	if (!s2 || !s1)
		return (NULL);
	while (s1[var.i++])
		var.len++;
	while (s2[var.j++])
		var.len++;
	var.str = (char *)malloc(sizeof(char) * (var.len + 1));
	if (!var.str)
	{
		free(s1);
		return (NULL);
	}
	var.i = 0;
	var.j = 0;
	while (s1[var.i])
		var.str[var.n++] = s1[var.i++];
	while (s2[var.j] && n--)
		var.str[var.n++] = s2[var.j++];
	var.str[var.n] = 0;
	free(s1);
	return ((char *)var.str);
}

t_gnl_list	*ft_lstnew(int fd)
{
	t_gnl_list	*new;

	new = (t_gnl_list *)malloc(sizeof(t_gnl_list));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->eof = 0;
	new->next = NULL;
	new->backup = NULL;
	return (new);
}

void	lst_delone(t_gnl_list *remove, t_gnl_list **head, t_gnl_list *tmp)
{
	tmp = *head;
	if (!tmp)
		return ;
	if (remove == tmp)
	{
		*head = tmp->next;
		if (remove->backup)
			free(remove->backup);
		free(remove);
		return ;
	}
	while (tmp && remove)
	{
		if (tmp->next == remove)
		{
			tmp->next = remove->next;
			if (remove->backup)
				free(remove->backup);
			free(remove);
			break ;
		}
		tmp = tmp->next;
	}
}

int	find_newline(char *str, t_gnl_list *tmp)
{
	t_var	var;

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
			return (var.n + 1);
		}
		var.n++;
	}
	return (0);
}
