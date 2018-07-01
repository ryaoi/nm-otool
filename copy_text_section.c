/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_text_section.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/01 15:17:27 by ryaoi             #+#    #+#             */
/*   Updated: 2018/07/01 18:42:14 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int				copy_text_section(t_filenm **file, void *ptr, \
									t_textinfo *textinfo)
{
	if (!((*file)->text = ft_memalloc(textinfo->size)))
		return (EXIT_FAILURE);
	ft_memcpy((*file)->text, \
			(void *)ptr + textinfo->offset, textinfo->size);
	(*file)->text_start_offset = textinfo->start_offset;
	(*file)->text_size = textinfo->size;
	free(textinfo);
	return (EXIT_SUCCESS);
}
