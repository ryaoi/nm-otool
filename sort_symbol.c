/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_symbol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/17 14:06:22 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/17 14:18:47 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		swap_symbol(t_symbol **ptr, t_symbol **next)
{
	char		*tmp_name;
	uint8_t		tmp_type;
	uint64_t	tmp_value;

	if (!(tmp_name = ft_strdup((*ptr)->name)))
		return (EXIT_FAILURE);
	free((*ptr)->name);
	if (!((*ptr)->name = ft_strdup((*next)->name)))
		return (EXIT_FAILURE);
	free((*next)->name);
	if (!((*next)->name = ft_strdup(tmp_name)))
		return (EXIT_FAILURE);
	free(tmp_name);
	tmp_type = (*ptr)->type;
	(*ptr)->type = (*next)->type;
	(*next)->type = tmp_type;
	tmp_value = (*ptr)->value;
	(*ptr)->value = (*next)->value;
	(*next)->value = tmp_value;
	return (EXIT_SUCCESS);
}

int				sort_symbol(t_symbol **sym)
{
	t_symbol	*ptr;
	t_symbol	*next;

	ptr = *sym;
	while (ptr->next != NULL)
	{
		next = ptr->next;
		while (next != NULL)
		{
			if (ft_strcmp(ptr->name, next->name) > 0)
			{
				if ((swap_symbol(&ptr, &next)) < 0)
					return (EXIT_FAILURE);
			}
			next = next->next;
		}
		ptr = ptr->next;
	}
	return (EXIT_SUCCESS);
}