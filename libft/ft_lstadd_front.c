/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:47:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 20:36:58 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
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
    ft_lstadd_front(&list, node1);
    ft_lstadd_front(&list, node2);
    ft_lstadd_front(&list, node3);

	while (list != NULL) {
        printf("%d ", list->content);
        list = list->next;
    }
    printf("\n");
	
	// Free memory
    free(node1);
    free(node2);
    free(node3);
	return (0);
}*/
