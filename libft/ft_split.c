/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:46:19 by thtinner          #+#    #+#             */
/*   Updated: 2025/03/12 17:56:46 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static char	*strallocate(char *start, char *end)
{
	int		len;
	int		i;
	char	*word;

	len = end - start;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static void	ft_free(char **arr, int i)
{
	while (i--)
		free(arr[i]);
	free(arr);
}

static int	count_words(char *str, char c)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str)
		{
			count++;
			while (*str && *str != c)
				str++;
		}
	}
	return (count);
}

char	**ft_split(char const *str, char c)
{
	char	**result;
	char	*start;
	int		i;

	result = malloc(sizeof(char *) * (count_words((char *)str, c) + 1));
	if (!result || !str)
		return (NULL);
	i = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str)
		{
			start = (char *)str;
			while (*str && *str != c)
				str++;
			result[i] = strallocate(start, (char *)str);
			if (!result[i])
				return (ft_free(result, i), NULL);
			i++;
		}
	}
	result[i] = NULL;
	return (result);
}
/*
#include <stdio.h>
int main()
{
    char str[] = "Hello world this is a test";
    char **words = ft_split(str, ' ');
    int i = 0;

    if (!words)
    {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    while (words[i])
    {
        printf("Word %d: %s\n", i, words[i]);
        free(words[i]);
        i++;
    }
    free(words);

    return 0;
}//*/
