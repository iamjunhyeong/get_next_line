/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 21:33:46 by junhyeop          #+#    #+#             */
/*   Updated: 2023/12/30 00:08:43 by junhyeop         ###   ########.fr       */
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
	size_t		len;
	size_t		n;
	char		*str;
}	t_var;

typedef struct s_strjoin_var
{
	size_t		len;
	size_t		n;
	size_t		i;
	size_t		j;
	char		*str;
}	t_strjoin_var;

typedef struct s_gnl_list
{
	int			fd;
	int			eof;
	char		*backup;
	struct s_gnl_list	*next;
}	t_gnl_list;

typedef struct s_gnl_var
{
	int			found;
	char		*line;
	t_gnl_list	*tmp;
}	t_gnl_var;

t_gnl_list	*ft_lstnew(int fd);
t_gnl_list	*find_fd(t_gnl_list **head, int fd, t_gnl_list *tmp);
void		lst_delone(t_gnl_list *remove, t_gnl_list **head, t_gnl_list *tmp);
char		*read_line_ext(t_var *var, char *line, t_gnl_list *tmp, int *found);
char		*get_next_line(int fd);
char		*ft_strndup(const char *str, size_t n);
char		*ft_strjoin(char *s1, char *s2, int n);
char		*read_line(t_gnl_list *tmp, char *line, int fd, int found);
int			find_newline(char *line, t_gnl_list *tmp);
char		*load_backup(t_gnl_list *tmp, int *found);

#endif