/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mget_next_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:15:51 by junhyeop          #+#    #+#             */
/*   Updated: 2023/12/23 22:53:08 by junhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
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

char	*ft_strjoin(char *s1, char const *s2, size_t n)
{
	t_var	var;

	var = (struct s_var){0, 0, NULL};
	if (!s2)
		return (NULL);
	if (!s1)
		s1 = ft_strndup("", 0);
	while (s1[var.n++])
		var.len++;
	var.n = 0;
	while (s2[var.n++])
		var.len++;
	var.str = (char *)malloc(sizeof(char) * (var.len + 1));
	if (!var.str)
		return (NULL);
	var.n = 0;
	while (*s1)
		var.str[var.n++] = *s1++;
	while (*s2 && n--)
		var.str[var.n++] = *s2++;
	var.str[var.n++] = *s2;
	var.str[var.n] = 0;
	free(s1);
	return ((char *)var.str);
}

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
			if (str[var.n + 1] != 0)
				tmp->backup = ft_strndup(&str[var.n + 1], BUFFER_SIZE);
			return (var.n);
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
	var.len = find_newline(var.str, tmp);
	if (var.len)
	{
		*found = 1;
		copy = ft_strndup(var.str, var.len + 1);
		free(var.str);
		return (copy);
	}
	copy = ft_strndup(var.str, BUFFER_SIZE);
	free(var.str);
	// free(tmp->backup);
	// tmp->backup = NULL;
	return (copy);
}

char	*read_line(t_gnl_list *tmp, char **line, int fd)
{
	t_var	var;

	while (1)
	{
		var.str = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!var.str)
			return (NULL);
		var.n = read(fd, var.str, BUFFER_SIZE);
		if (var.n <= 0)
			break ;
		var.str[var.n] = '\0';
		var.len = find_newline(var.str, tmp); 
		if (var.len) // newline exists
		{
			*line = ft_strjoin(*line, var.str, var.len);
			free(var.str);
			return (*line);
		}
		*line = ft_strjoin(*line, var.str, BUFFER_SIZE);
	}
	free(var.str);
	tmp->eof = 1;
	return (NULL);
}

// char	*newline_exists(int len, char **line, char **str);
// {
	
// }

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

t_gnl_list	*find_fd(t_gnl_list **head, int fd, t_gnl_list *tmp)
{
	if (!*head)
		*head = ft_lstnew(fd);
	if (!*head)
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

void	lst_delone(t_gnl_list *remove, t_gnl_list *head)
{
	if (!head)
		return ;
	if (remove == head)
	{
		head = head->next;
		if (remove->backup)
			free(remove->backup);
		free(remove);
		return ;
	}
	while (head && remove)
	{
		if (head->next == remove)
		{
			head->next = remove->next;
			if (remove->backup)
				free(remove->backup);
			free(remove);
			return ;
		}
		head = head->next;
	}
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*head;
	t_gnl_list			*tmp;
	char				*str;
	int 				found;
	
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	tmp = find_fd(&head, fd, NULL);
	if (!tmp)
		return (NULL);
	str = NULL;
	found = 0;
	if (tmp->backup)
		str = load_backup(tmp, &found);
	if (!found || !str)
		str = read_line(tmp, &str, fd);
	if (tmp->eof)
		lst_delone(tmp, head);

	return (str);
}

#include <stdio.h>
int	main(void)
{
	int		fd1;
	int		fd2;
//	int		fd3;
//	int		fd4;
	char	*line1;
	char	*line2;
//	char	*line3;
//	char	*line4;
	// size_t	len = 0;

	fd1 = open("testfile1", O_RDONLY);
	fd2 = open("testfile2", O_RDONLY);
	do
	{
		line1 = get_next_line(fd1);
		printf("fd1 : %d\n%s", fd1, line1);
		printf("------------\n");
		line2 = get_next_line(fd2);
		printf("fd2 : %d\n%s", fd2, line2);
		printf("------------\n");
	} while (line1 && line2);
	close(fd1);
	close(fd2);
/*
	fd1 = open("testfile1", O_RDONLY);
	line1 = get_next_line(fd1);
	printf("fd1 : %d\n%s", fd1, line1);
	printf("------------\n");
	close(fd1);
	line1 = get_next_line(fd1);
	printf("fd1 : %d\n%s", fd1, line1);
	printf("------------\n");
*/
/*
	fd3 = open("testfile3", O_RDONLY);
	line3 = get_next_line(fd3);
	printf("fd3 : %d\n%s", fd3, line3);
	printf("------------\n");
	close(fd3);
*/
/*
	fd4 = open("testfile4", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	len = 1000000;
	line4 = malloc(len);
	memset(line4, '0', len);
	write(fd4, line4, len);
	free(line4);
	close(fd4);
	fd4 = open("testfile4", O_RDONLY);
	do
	{
		line4 = get_next_line(fd4);
		printf("fd4 : %d\n%s", fd4, line4);
		printf("------------\n");
	} while (line4);
	close(fd4);
*/
}