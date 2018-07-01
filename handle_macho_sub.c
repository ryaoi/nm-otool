/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho_sub.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 18:30:15 by ryaoi             #+#    #+#             */
/*   Updated: 2018/07/01 14:54:04 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int			get_symbol_and_text(t_filenm **file, void *ptr,\
								struct mach_header_64 *header64,\
								struct mach_header *header)
{
	if ((get_symbol(file, (*file)->secindex, ptr)) == EXIT_FAILURE)
		return (corrupt_msg(file));
	(*file)->filesize = (*file)->real_filesize;
	if ((((*file)->type_flag & IS_64) > 1) && (*file)->type_flag & IS_OTOOL)
		return (get_text(file, ptr, header64, NULL));
	else if ((*file)->type_flag & IS_OTOOL)
		return (get_text(file, ptr, NULL, header));
	return (EXIT_SUCCESS);
}
