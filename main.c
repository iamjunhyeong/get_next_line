#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
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

	if ((copy = malloc(len + 1)) == NULL)
		return (NULL);

	while (len-- > 0)
		copy[i++] = *str++;
	copy[len] = '\0';
	return (copy);
}

char	*ft_strjoin(char **s1, char const *s2, size_t n)
{
	size_t	len;
	size_t	i;
	char	*str;

	if (!s2)
		return (NULL);
	if (!s1)
		s1 = ft_strndup("", 0);
	i = 0;
	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (*s1)
		str[i++] = *s1++;
	while (*s2 && *s2 != '\n' && n--)
		str[i++] = *s2++;
	str[i++] = *s2;
	str[i] = 0;
	return ((char *)str);
}

size_t	ft_strlen(const char *s)
{
	size_t	n;

	n = 0;
	while (s[n] != 0)
		n++;
	return (n);
}

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

	while (1)
	{
		line = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (!line)
			return (0);
		n = read(fd, line, BUFFER_SIZE);
		if (n <= 0)
			break ;
		line[n] = '\0';
		len = find_newline(line, tmp, 0);
		if (len)
		{
			*str = ft_strjoin(*str, line, len);
			return (1);
		}
		*str = ft_strjoin(*str, line, BUFFER_SIZE);
	}
	free(line);
	tmp->eof = 1;
	return (1);
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
	t_gnl_list	*hhead;

	hhead = head;
	if (!head || !hhead)
		return ;
	if (remove == hhead)
	{
		hhead = hhead->next;
		if (remove->backup)
			free(remove->backup);
		free(remove);
		return ;
	}
	while (hhead && remove)
	{
		if (hhead->next == remove)
		{
			hhead->next = remove->next;
			if (remove->backup)
				free(remove->backup);
			free(remove);
			return ;
		}
		hhead = hhead->next;
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
		if (!ft_strlen(tmp->backup))
			tmp->backup = NULL;
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
		str = load_backup(tmp);
	while (1)
	{
		if (!read_line(tmp, &str, fd) && !str)
			return (NULL);
		if (tmp->eof || str)
		{
			if (tmp->eof)
				lst_delone(tmp, head);
			return (str);
		}
	}
}

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

