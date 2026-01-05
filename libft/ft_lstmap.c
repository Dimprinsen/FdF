/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:47:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/21 15:20:49 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f) (void *), void (*del) (void *))
{
	t_list	*new_lst;
	t_list	*new_node;
	void	*new_content;

	new_lst = NULL;
	while (lst)
	{
		new_content = (*f)(lst->content);
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			del(new_content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_node);
		lst = lst->next;
	}
	return (new_lst);
}
/*
void *to_uppercase(void *content)
{
    char *str = (char *)content;
    size_t len = strlen(str);
    char *upper_str = malloc(len + 1);
    if (!upper_str)
        return NULL;
    for (size_t i = 0; i < len; i++)
        upper_str[i] = (char)toupper((unsigned char)str[i]);
    upper_str[len] = '\0';
    return upper_str;
}
void del(void *content)
{
    free(content);
}
#include <stdio.h>
int main(void)
{
    // Create original nodes
    t_list *node1 = ft_lstnew(strdup("hello"));
    t_list *node2 = ft_lstnew(strdup("world"));
    t_list *node3 = ft_lstnew(strdup("libft"));

    // Link nodes
    ft_lstadd_back(&node1, node2);
    ft_lstadd_back(&node1, node3);

    // Map new list with uppercased strings
    t_list *upper_list = ft_lstmap(node1, to_uppercase, del);

    // Print original list
    printf("Original list:\n");
    t_list *tmp = node1;
    while (tmp)
    {
        printf("%s\n", (char *)tmp->content);
        tmp = tmp->next;
    }

    // Print uppercased list
    printf("\nUppercased list:\n");
    tmp = upper_list;
    while (tmp)
    {
        printf("%s\n", (char *)tmp->content);
        tmp = tmp->next;
    }

    // Free both lists
    ft_lstclear(&node1, del);
    ft_lstclear(&upper_list, del);

    return 0;
}//*/