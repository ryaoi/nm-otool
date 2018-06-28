/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filenm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:32:54 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/28 21:57:40 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int					count_filenm(t_filenm *file)
{
	int				ret;

	ret = 0;
	while (file != NULL)
	{
		file = file->next;
		ret++;
	}
	return (ret);
}

static void			free_symbol(t_symbol **sym)
{
	t_symbol		*ptr;
	t_symbol		*next;

	ptr = *sym;
	while (ptr != NULL)
	{
		next = ptr->next;
		free(ptr->name);
		free(ptr);
		ptr = next;
	}
}

void				free_filenm(t_filenm **head)
{
	t_filenm		*ptr;
	t_filenm		*next;

	ptr = *head;
	while (ptr != NULL)
	{
		next = ptr->next;
		free(ptr->filename);
		if (ptr->err_msg != NULL)
			free(ptr->err_msg);
		else
			free_symbol(&((ptr)->sym));
		if (ptr->secindex != NULL)
			free(ptr->secindex);
		if (ptr->text != NULL)
			free(ptr->text);
		free(ptr);
		ptr = next;
	}
}

t_filenm			*add_filenm(t_filenm **head, char *name, int is_otool)
{
	t_filenm		*new;
	t_filenm		*ptr;

	if (!(new = malloc(sizeof(t_filenm))))
		return (NULL);
	*new = (t_filenm){NULL, 0, NULL, NULL, NULL, NULL, 0, 0, 0, 0, NULL};
	if (is_otool)
		new->type_flag += is_otool;
	if (!(new->filename = ft_strdup(name)))
		return (NULL);
	if (*head == NULL)
		*head = new;
	else
	{
		ptr = *head;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
	}
	return (new);
}
