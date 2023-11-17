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

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUF_SIZE 42
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>

typedef struct s_gnl_list
{
	int			fd;
	int			eof;
	char		*backup;
	t_gnl_list	*next;
}	t_gnl_list;

typedef struct s_str_list
{
	char		*s;
	t_str_list	*next;
}	t_str_list;

#endif