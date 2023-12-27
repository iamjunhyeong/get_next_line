/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 21:33:46 by junhyeop          #+#    #+#             */
/*   Updated: 2023/11/17 20:23:20 by junhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# ifndef BUFFER_SIZE
	# define BUFFER_SIZE 1
#endif
# define _ERROR -1
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct s_var
{
	int		len;
	int		n;
	char	*str;
}	t_var;

typedef struct s_gnl_list
{
	int			fd;
	int			eof;
	char		*backup;
	struct s_gnl_list	*next;
}	t_gnl_list;

t_gnl_list	*ft_lstnew(int fd);
t_gnl_list	*find_fd(t_gnl_list **head, int fd, t_gnl_list *tmp);
t_gnl_list	*lst_delone(t_gnl_list *remove, t_gnl_list *head, t_gnl_list *tmp);
char		*get_next_line(int fd);
char		*ft_strndup(const char *str, size_t n);
char		*ft_strjoin(char *s1, char const *s2, size_t n);
char		*read_line(t_gnl_list *tmp, char **line, int fd, int found);
int			find_newline(char *line, t_gnl_list *tmp);
char		*load_backup(t_gnl_list *tmp, int *found);

#endif