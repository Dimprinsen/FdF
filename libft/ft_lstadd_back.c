/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:47:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 22:50:42 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst)
		return ;
	if (!*lst)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;
}
/*
int	main(void)
{
	t_list *list = NULL;

    // Create nodes
    t_list *node1 = malloc(sizeof(t_list));
    node1->content = (void *)1;
    node1->next = NULL;

    t_list *node2 = malloc(sizeof(t_list));
    node2->content = (void *)2;
    node2->next = NULL;

    t_list *node3 = malloc(sizeof(t_list));
    node3->content = (void *)3;
    node3->next = NULL;

    // Add nodes to the list
    printf("add start:\n");
    ft_lstadd_front(&list, node1);
    ft_lstadd_front(&list, node2);
    ft_lstadd_front(&list, node3);

	while (list != NULL) {
        printf("%d ", list->content);
        list = list->next;
    }
    printf("\n");

	// Test size
	t_list *ptrs = node3;
	printf("size:%d\n", ft_lstsize(ptrs));

	// Test add to last
	t_list *ptr2 = node3;
	t_list *node4 = malloc(sizeof(t_list));
	node4->content = (void *)4;
	node4->next = NULL;
	ft_lstadd_back(((void*)0), node4);
    printf("add end:\n");
	while (ptr2 != NULL) {
        printf("%d ", ptr2->content);
        ptr2 = ptr2->next;
    }
    printf("\n");

	// Test size
	t_list *ptr = node3;
	printf("size:%d\n", ft_lstsize(ptr));

	// Test last
	t_list *ptr3 = node3;
	t_list *last = ft_lstlast(ptr3);
	printf("last:%d\n", last->content);	

	// Free memory
    free(node1);
    free(node2);
    free(node3);
    free(node4);
	return (0);
}*/
	//takes lst, adress to pointer of first node
	//takes new, adress of pointer to node to be added
