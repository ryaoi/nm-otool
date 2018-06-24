/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/17 16:55:37 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/24 22:17:16 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void					check_cpu_type(struct fat_arch *fatarch,\
											int *cpu_type, int *offset, int *offset_powerpc)
{
	if (swap32(fatarch->cputype) == CPU_TYPE_X86_64)
	{
		*cpu_type = CPU_TYPE_X86_64;
		*offset = swap32(fatarch->offset);
	}
	if (swap32(fatarch->cputype) == CPU_TYPE_I386 \
		&& *cpu_type != CPU_TYPE_X86_64)
	{
		*cpu_type = CPU_TYPE_I386;
		*offset = swap32(fatarch->offset);
	}
	if (swap32(fatarch->cputype) == CPU_TYPE_POWERPC)
	{
		*offset_powerpc = swap32(fatarch->offset);
	}
}

static void					choose_cpu_type(t_filenm **file, int cpu_type, void *ptr, int offset_powerpc)
{
	t_filenm 				*file_ptr;
	char					*name1;
	char					*name2;

	if (cpu_type & CPU_TYPE_X86_64)
	{
		(*file)->type_flag -= IS_FAT;
		(*file)->type_flag += IS_64;
	}
	if (offset_powerpc != 0)
	{
		if ((*file)->type_flag & IS_OTOOL)
			name1 = ft_strjoin((*file)->filename, " (architecture ppc)");
		else
			name1 = ft_strjoin((*file)->filename, " (for architecture ppc)");
		if ((*file)->type_flag & IS_OTOOL)
			name2 = ft_strjoin((*file)->filename, " (architecture i386)");
		else
			name2 = ft_strjoin((*file)->filename, " (for architecture i386)");	
		free((*file)->filename);
		(*file)->filename = name1;
		file_ptr = add_filenm(file, name2, (*file)->type_flag & IS_OTOOL);
		(*file)->type_flag = IS_POWERPC + (*file)->type_flag & IS_OTOOL;
		file_ptr->type_flag = (*file)->type_flag - IS_POWERPC;
		handle_powerpc(file, ptr + offset_powerpc);
		free(name2);
		*file = file_ptr;
	}
}

int							handle_fat(t_filenm **file, void *ptr)
{
	struct fat_arch			*fatarch;
	struct fat_header		*fatheader;
	uint32_t				i;
	int						cpu_type;
	int						offset;
	int						offset_powerpc;

	i = 0;
	offset_powerpc = 0;
	if ((*file)->type_flag & (IS_FAT))
	{
		fatheader = (struct fat_header *)ptr;
		fatarch = (void *)ptr + sizeof(struct fat_header);
		while (i < swap32(fatheader->nfat_arch))
		{
			check_cpu_type(fatarch, &cpu_type, &offset, &offset_powerpc);
			fatarch = (void *)fatarch + sizeof(struct fat_arch);
			i++;
		}
		choose_cpu_type(file, cpu_type, ptr, offset_powerpc);
		if ((handle_macho(file, ptr + offset)) < 0)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
