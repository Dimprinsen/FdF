
#include "fdf.h"

char    *ft_strncpy(char *dest, char *src, int n)
{
    int i = 0;
    while (i < n && src[i])
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return (dest);
}
int ft_isspace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return (1);
    else
        return (0);
}
char    **ft_split(char *str)
{
    int i = 0;
    int j = 0;
    int wc = 0;
    while (str[i])
    {
        while (str[i] && ft_isspace(str[i]))
            i++;
        if (str[i])
            wc++;
        while (str[i] && !ft_isspace(str[i]))
            i++;
    }
    char **result = malloc(sizeof(char *) * (wc + 1));
    i = 0;
    while (str[i])
    {
        while (str[i] && ft_isspace(str[i]))
            i++;
        int start = i;
        while (str[i] && !ft_isspace(str[i]))
            i++;
        int end = i;
        if (end > start)
        {
            result[j] = malloc(sizeof(char) * (end - start + 1));
            ft_strncpy(result[j], &str[start], end - start);
            j++;
        }
    }
    result[j] = NULL;
    return(result);
}
