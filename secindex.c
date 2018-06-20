/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secindex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 20:57:48 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/20 15:27:05 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int				init_secindex(t_secindex **head)
{
	t_secindex	*new;

	if (!(new = malloc(sizeof(t_secindex))))
		return (EXIT_FAILURE);
	new->index = 1;
	new->data_bss = 0;
	new->text_text = 0;
	new->data_data = 0;
	new->symtab_value = NULL;
	*head = new;
	return (EXIT_SUCCESS);
}
