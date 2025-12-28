/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/08/25 20:55:41 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# define FD_LIMIT 4096
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

char	*get_next_line(int fd);
int		gnl_slen(char *str);
char	*gnl_strchr(char *s, int c);
char	*gnl_strdup(char *s);
char	*gnl_substr(char *s, unsigned int start, size_t len);
char	*gnl_strjoin(char *s1, char *s2);
#endif
