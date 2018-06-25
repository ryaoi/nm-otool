/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 16:45:49 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/25 14:55:58 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			exec_handler(t_filenm **file, void *ptr, long magic_number)
{
	if (((*file)->type_flag & IS_FAT))
	{
		if ((handle_fat(file, ptr, NULL)) < 0)
			return (EXIT_FAILURE);
	}
	else if (((*file)->type_flag & IS_AR))
	{
		if ((handle_ar(file, ptr, NULL)) < 0)
			return (EXIT_FAILURE);
	}
	else if (((*file)->type_flag & IS_64) || (uint32_t)magic_number == MH_MAGIC\
			|| (uint32_t)magic_number == MH_CIGAM)
	{
		if ((handle_macho(file, ptr)) < 0)
			return (EXIT_FAILURE);
	}
	else if ((*file)->type_flag & IS_OTOOL)
		(*file)->err_msg = \
			ft_strdup(" is not an object file\n");
	else
		(*file)->err_msg = \
			ft_strdup("The file was not recognized as a valid object file\n");
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
	if (exec_handler(file, ptr, magic_number) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
