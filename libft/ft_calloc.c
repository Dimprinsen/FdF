/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:03:26 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/13 16:50:46 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;

	if (nmemb != 0 && size > SIZE_MAX / nmemb)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}
/*
// Dummy ft_bzero implementation
void ft_bzero(void *s, size_t n)
{
    unsigned char *p = s;
    while (n--)
        *p++ = 0;
}

void test_zeroed_memory(void *ptr, size_t size, const char *label)
{
    unsigned char *bytes = (unsigned char *)ptr;
    for (size_t i = 0; i < size; ++i)
    {
        if (bytes[i] != 0)
        {
            printf("❌ %s not zeroed at byte %zu\n", label, i);
            return;
        }
    }
    printf("✅ %s is zero-initialized\n", label);
}

int main(void)
{
    // Normal test case
    size_t count = 10;
    size_t size = sizeof(int);

    int *std = (int *)calloc(count, size);
    int *custom = (int *)ft_calloc(count, size);

    test_zeroed_memory(std, count * size, "calloc");
    test_zeroed_memory(custom, count * size, "ft_calloc");

    free(std);
    free(custom);

    // Zero allocation test: calloc(0, 10)
    void *z1 = calloc(0, 10);
    void *fz1 = ft_calloc(0, 10);
    printf("calloc(0, 10): ptr = %p\n", z1);
    printf("ft_calloc(0, 10): ptr = %p\n", fz1);
    free(z1);
    free(fz1);

    // Zero allocation test: calloc(10, 0)
    void *z2 = calloc(10, 0);
    void *fz2 = ft_calloc(10, 0);
    printf("calloc(10, 0): ptr = %p\n", z2);
    printf("ft_calloc(10, 0): ptr = %p\n", fz2);
    free(z2);
    free(fz2);

    // Overflow test (should return NULL)
    size_t big = SIZE_MAX / 2 + 1;
    void *ov1 = calloc(big, 2);
    void *fov1 = ft_calloc(big, 2);
    printf("calloc overflow: %s\n", ov1 == NULL ? "✅ NULL" : "❌ not NULL");
    printf("ft_calloc overflow: %s\n", fov1 == NULL ? "✅ NULL" : "❌ not NULL");
    free(ov1);
    free(fov1);

    return 0;
}//*/	
