/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filenm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:32:54 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/16 21:18:40 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int					free_filenm(t_filenm **head)
{
	t_filenm		*ptr;
	t_filenm		*next;

	ptr = *head;
	while (ptr != NULL)
	{
		next = ptr->next;
		free(ptr->filename);
		//free sym
		if (ptr->err_msg != NULL)
			free(ptr->err_msg);
		free(ptr);
		ptr = next;	
	}
	free(head);
	return(EXIT_SUCCESS);
}

t_filenm			*add_filenm(t_filenm **head, char *name)
{
	t_filenm		*new;
	t_filenm		*ptr;

	if (!(new = malloc(sizeof(t_filenm))))
		return (NULL);
	*new = (t_filenm){NULL, 0, NULL, NULL, NULL, NULL};
	if (!(new->filename = ft_strdup(name)))
		return (NULL);
	if (*head == NULL)
		*head = new;
	else
	{
		ptr = *head;
		while(ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
	}
	return (new);
}