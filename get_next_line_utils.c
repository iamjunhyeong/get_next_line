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
	copy = (char*)malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	while (len-- > 0 || *str)
		copy[i++] = *str++;
	copy[i] = '\0';
	return (copy);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;

	if (!dst)
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	i = 0;
	if (dstsize < dst_len + 1)
		return (dstsize + src_len);
	if (dstsize > dst_len + 1)
	{
		while (src[i] && dst_len + i + 1 < dstsize)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

char	*ft_strjoin(char *s1, char const *s2, int n)
{
	t_var	var;

	var = (struct s_var){0, 0, NULL};
	if (!s2)
		return (NULL);
	if (!s1)
		s1 = ft_strndup("", 0);
	if (!s1)
		return (NULL);
	while (s1[var.n++])
		var.len++;
	var.n = 0;
	while (s2[var.n++] && var.n < n)
		var.len++;
	var.str = (char *)malloc(sizeof(char) * (var.len + 1));
	if (!var.str)
		return (NULL);
	var.n = 0;
	while (*s1)
		var.str[var.n++] = *s1++;
	while (*s2 && n--)
		var.str[var.n++] = *s2++;
	var.str[var.n] = 0;
	return ((char *)var.str);
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

t_gnl_list	*lst_delone(t_gnl_list *remove, t_gnl_list *head, t_gnl_list *tmp)
{
	tmp = head;
	if (!tmp)
		return (NULL);
	if (remove == tmp)
	{
		tmp = tmp->next;
		if (remove->backup)
			free(remove->backup);
		free(remove);
		return (tmp);
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
	return(head);
}