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
		copy = ft_strndup(var.str, var.len);
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
		{
			free(var.str);
			return (NULL);
		}
	}
	return (line);
}

char	*read_line_ext(t_var *var, char *line, t_gnl_list *tmp, int *found)
{
	var->str[var->n] = '\0';
	var->len = find_newline(var->str, tmp);
	if (line == NULL)
		line = ft_strndup("", 0);
	if (line == NULL)
		return (NULL);
	if (var->len)
	{
		if (var->str[var->len])
		{
			tmp->backup = ft_strndup(&var->str[var->len], BUFFER_SIZE);
			if (tmp->backup == NULL)
				return (NULL);
		}
		line = ft_strjoin(line, var->str, var->len);
		free(var->str);
		if (!line)
			return (NULL);
		*found = 1;
		return (line);
	}
	line = ft_strjoin(line, var->str, BUFFER_SIZE);
	return (line);
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
	if (!var.line || var.tmp->eof)
		lst_delone(var.tmp, &head, NULL);
	return (var.line);
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
	} while (line1 || line2);
	close(fd1);
	close(fd2);
}
