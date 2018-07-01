/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat_sub.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 20:08:55 by ryaoi             #+#    #+#             */
/*   Updated: 2018/07/01 18:19:38 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int				with_ppc(t_filenm **file, void *ptr,\
												t_arch arch)
{
	if (arch.intel32_size != 0)
	{
		(*file)->filesize = arch.intel32_size;
		(*file)->real_filesize = arch.intel32_size;
		if ((handle_macho(file, ptr + arch.intel32_offset)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (arch.intel64_size != 0)
	{
		(*file)->filesize = arch.intel64_size;
		(*file)->real_filesize = arch.intel64_size;
		if ((handle_macho(file, ptr + arch.intel64_offset)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int				only_intel(t_filenm **file, void *ptr,\
												t_arch arch)
{
	if (arch.intel64_size != 0)
	{
		(*file)->filesize = arch.intel64_size;
		(*file)->real_filesize = arch.intel64_size;
		(*file)->type_flag = (*file)->type_flag & IS_OTOOL;
		(*file)->type_flag += IS_64;
		if ((handle_macho(file, ptr + arch.intel64_offset)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (arch.intel32_size != 0)
	{
		(*file)->filesize = arch.intel32_size;
		(*file)->real_filesize = arch.intel32_size;
		(*file)->type_flag = (*file)->type_flag & IS_OTOOL;
		if ((handle_macho(file, ptr + arch.intel32_offset)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int						handle_multiple_arch(t_filenm **file, void *ptr,\
												t_arch arch)
{
	if (arch.ppc32_size != 0)
	{
		if ((with_ppc(file, ptr, arch)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		if ((only_intel(file, ptr, arch)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
