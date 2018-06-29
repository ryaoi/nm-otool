/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/17 16:55:37 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/29 20:35:49 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int64_t 				align_fat(int64_t value, uint32_t align)
{
	return ((((value) + ((align) - 1)) & ~((align) - 1)));
}

static void					check_cpu_type(struct fat_arch *fatarch,\
											t_arch *arch)
{
	if (swap32(fatarch->cputype) == CPU_TYPE_X86_64)
	{
		arch->intel64_size = \
			align_fat(swap32(fatarch->size), 1 << swap32(fatarch->align));
		arch->intel64_offset = swap32(fatarch->offset);
	}
	else if (swap32(fatarch->cputype) == CPU_TYPE_I386)
	{
		arch->intel32_size = \
			align_fat(swap32(fatarch->size), 1 << swap32(fatarch->align));
		arch->intel32_offset = swap32(fatarch->offset);
	}
	else if (swap32(fatarch->cputype) == CPU_TYPE_POWERPC)
	{
		arch->ppc32_size = \
			align_fat(swap32(fatarch->size), 1 << swap32(fatarch->align));
		arch->ppc32_offset = swap32(fatarch->offset);
	}
	else if (swap32(fatarch->cputype) == CPU_TYPE_POWERPC64)
	{
		arch->ppc64_size = \
			align_fat(swap32(fatarch->size), 1 << swap32(fatarch->align));
		arch->ppc64_offset = swap32(fatarch->offset);
	}
}

static void					choose_cpu_type(t_filenm **file, \
											t_arch *arch, void *ptr)
{
	t_filenm				*file_ptr;
	char					*name1;
	char					*name2;

	if (arch->ppc32_size != 0)
	{
		if ((*file)->type_flag & IS_OTOOL)
			name1 = ft_strjoin((*file)->filename, ARCH_PPC_NM);
		else
			name1 = ft_strjoin((*file)->filename, ARCH_PPC_OTOOL);
		if ((*file)->type_flag & IS_OTOOL)
			name2 = ft_strjoin((*file)->filename, ARCH_I386_NM);
		else
			name2 = ft_strjoin((*file)->filename, ARCH_I386_OTOOL);
		free((*file)->filename);
		(*file)->filename = name1;
		file_ptr = add_filenm(file, name2, (*file)->type_flag & IS_OTOOL);
		(*file)->type_flag = IS_POWERPC + (*file)->type_flag & IS_OTOOL;
		file_ptr->type_flag = (*file)->type_flag - IS_POWERPC - WAS_FAT;
		(*file)->filesize = arch->ppc32_size;
		(*file)->real_filesize = arch->ppc32_size;
		handle_powerpc(file, ptr + arch->ppc32_offset);
		free(name2);
		*file = file_ptr;
	}
}
static void					filesize_checker(t_filenm **file,\
										struct fat_arch *fatarch,\
										uint32_t *i,
										struct fat_header *fatheader)
{
	if (*i == 0)
		(*file)->filesize -= swap32(fatarch->offset);
	if (*i != swap32(fatheader->nfat_arch) - 1)
		(*file)->filesize -=\
			 (align_fat(swap32(fatarch->size), 1 << swap32(fatarch->align)));
	else
		(*file)->filesize -= swap32(fatarch->size);
	*i = *i + 1;
}

int							handle_fat(t_filenm **file, void *ptr,\
										struct fat_header *fatheader)
{
	struct fat_arch			*fatarch;
	uint32_t				i;
	t_arch					arch;				

	i = 0;
	ft_bzero(&arch, sizeof(t_arch));
	if ((*file)->type_flag & (IS_FAT))
	{
		fatheader = (struct fat_header *)ptr;
		fatarch = (void *)ptr + sizeof(struct fat_header);
		while (i < swap32(fatheader->nfat_arch) && (*file)->filesize > 0)
		{
			check_cpu_type(fatarch, &arch);
			filesize_checker(file, fatarch, &i, fatheader);
			fatarch = (void *)fatarch + sizeof(struct fat_arch);
		}
		if (i != swap32(fatheader->nfat_arch) || (*file)->filesize != 0)
		{
			(*file)->err_msg = ft_strdup(ERR_MSG_CORRUPT);
			return (EXIT_FAILURE);
		}
		choose_cpu_type(file, &arch, ptr);
		if ((handle_multiple_arch(file,ptr,arch)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
