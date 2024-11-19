/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttinnerh <ttinnerh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 00:06:02 by ttinnerh          #+#    #+#             */
/*   Updated: 2023/11/18 03:35:18 by ttinnerh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# include <stdlib.h> //malloc, free, calloc
# include <unistd.h> //write
# include <fcntl.h> //FD stuff

char	*get_next_line(int fd);
int		ft_strlen(char *str);
char	*ft_strchr(char *s, int c);
char	*ft_strdup(char *s);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
char	**ft_split(char *str);

#endif
