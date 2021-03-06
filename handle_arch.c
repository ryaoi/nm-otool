/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 16:45:49 by ryaoi             #+#    #+#             */
/*   Updated: 2018/07/01 18:15:10 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			exec_handler(t_filenm **file, void *ptr, long magic_number)
{
	if (((*file)->type_flag & IS_FAT))
	{
		if ((handle_fat(file, ptr, NULL)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (((*file)->type_flag & IS_AR))
	{
		if ((handle_ar(file, ptr, NULL, (void *)ptr + sizeof(MH_AR_64)))\
			== EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (((*file)->type_flag & IS_64) || (uint32_t)magic_number == MH_MAGIC\
			|| (uint32_t)magic_number == MH_CIGAM)
	{
		if ((handle_macho(file, ptr)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if ((*file)->type_flag & IS_OTOOL)
		(*file)->err_msg = \
			ft_strdup(ERR_MMAP_OTOOL);
	else
		(*file)->err_msg = \
			ft_strdup(ERR_MMAP_NM);
	return (EXIT_SUCCESS);
}

int					handle_arch(t_filenm **file, void *ptr)
{
	long			magic_number;

	magic_number = *(long *)ptr;
	if ((uint32_t)magic_number == MH_MAGIC_64 \
		|| (uint32_t)magic_number == MH_CIGAM_64\
		|| (uint32_t)magic_number == FAT_MAGIC_64 \
		|| (uint32_t)magic_number == FAT_CIGAM_64)
		(*file)->type_flag += IS_64;
	if (magic_number == (long)MH_AR_64 || magic_number == (long)MH_RA_64)
		(*file)->type_flag += IS_AR;
	if ((uint32_t)magic_number == FAT_MAGIC\
		|| (uint32_t)magic_number == FAT_CIGAM
		|| (uint32_t)magic_number == FAT_MAGIC_64\
		|| (uint32_t)magic_number == FAT_CIGAM_64)
		(*file)->type_flag += IS_FAT;
	if (exec_handler(file, ptr, magic_number) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
