/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:01:49 by thtinner          #+#    #+#             */
/*   Updated: 2026/01/05 20:22:45 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*fill_buffer(int fd, char *remainder, char *buffer)
{
	char	*tmp;
	ssize_t	fd_read;

	fd_read = 1;
	while (fd_read > 0)
	{
		fd_read = read(fd, buffer, BUFFER_SIZE);
		if (fd_read == -1)
			return (free (remainder), NULL);
		else if (fd_read == 0)
			break ;
		buffer[fd_read] = '\0';
		if (!remainder)
			remainder = ft_strdup("");
		tmp = remainder;
		remainder = ft_strjoin(tmp, buffer);
		free(tmp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (remainder);
}

static char	*split_line(char *line)
{
	ssize_t	i;
	char	*remainder;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (NULL);
	remainder = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (!remainder || *remainder == '\0')
	{
		free(remainder);
		remainder = NULL;
		return (NULL);
	}
	line[i + 1] = '\0';
	return (remainder);
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;
	char		*buffer;
	char		*tmp;

	if (fd < 0 || read(fd, 0, 0) < 0 || BUFFER_SIZE <= 0)
		return (free(remainder), remainder = NULL, NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	remainder = fill_buffer(fd, remainder, buffer);
	free(buffer);
	if (!remainder)
		return (NULL);
	line = ft_strdup(remainder);
	if (!line)
		return (NULL);
	tmp = split_line(line);
	free(remainder);
	remainder = tmp;
	return (line);
}
/*
#include <stdio.h>
int	main(int argc, char *argv[])
{
	int		fd;
	char	*line;
	int		line_num;

	argc = 0;
	line_num = 1;
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open file");
		return (1);
	}

	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Line %d: \"%s\"\n", line_num++, line);
		free(line);
	}
	close(fd);
	return (0);
}//*/
