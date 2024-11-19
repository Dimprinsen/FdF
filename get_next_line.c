/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttinnerh <ttinnerh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 00:05:52 by ttinnerh          #+#    #+#             */
/*   Updated: 2023/10/10 00:05:55 by ttinnerh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*get_line_buffer(int fd, char *left_fd, char *buffer);
static char	*set_line(char *line_buffer);

char	*get_next_line(int fd)
{
	static char		*left_fd;
	char			*read_l;
	char			*buffer;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(left_fd);
		free(buffer);
		left_fd = NULL;
		buffer = NULL;
		return (NULL);
	}
	if (!buffer)
		return (NULL);
	read_l = get_line_buffer(fd, left_fd, buffer);
	free(buffer);
	buffer = NULL;
	if (!read_l)
		return (NULL);
	left_fd = set_line(read_l);
	return (read_l);
}

static char	*get_line_buffer(int fd, char *left_fd, char *buffer)
{
	ssize_t	read_check;
	char	*tmp;

	read_check = 1;
	while (read_check > 0)
	{
		read_check = read(fd, buffer, BUFFER_SIZE);
		if (read_check == -1)
		{
			free(left_fd);
			return (NULL);
		}
		else if (read_check == 0)
			break ;
		buffer[read_check] = 0;
		if (!left_fd)
			left_fd = ft_strdup("");
		tmp = left_fd;
		left_fd = ft_strjoin(tmp, buffer);
		free (tmp);
		tmp = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (left_fd);
}

static char	*set_line(char *line_buffer)
{
	char	*left_fd;
	ssize_t	i;

	i = 0;
	while (line_buffer[i] != '\n' && line_buffer[i] != '\0')
		i++;
	if (line_buffer[i] == 0)
		return (NULL);
	left_fd = ft_substr(line_buffer, i + 1, ft_strlen(line_buffer) - i);
	if (*left_fd == 0)
	{
		free(left_fd);
		left_fd = NULL;
	}
	line_buffer[i + 1] = 0;
	return (left_fd);
}
