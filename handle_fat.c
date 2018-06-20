/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/17 16:55:37 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/20 22:13:35 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void					check_cpu_type(struct fat_arch *fatarch,\
											int *cpu_type, int *offset)
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
}

static void					choose_cpu_type(t_filenm **file, int cpu_type)
{
	if (cpu_type == CPU_TYPE_X86_64)
	{
		(*file)->type_flag -= IS_FAT;
		(*file)->type_flag += IS_64;
	}
}

int							handle_fat(t_filenm **file, void *ptr)
{
	struct fat_arch			*fatarch;
	struct fat_header		*fatheader;
	uint32_t				i;
	int						cpu_type;
	int						offset;

	i = 0;
	if ((*file)->type_flag & (IS_FAT))
	{
		fatheader = (struct fat_header *)ptr;
		fatarch = (void *)ptr + sizeof(struct fat_header);
		while (i < swap32(fatheader->nfat_arch))
		{
			check_cpu_type(fatarch, &cpu_type, &offset);
			fatarch = (void *)fatarch + sizeof(struct fat_arch);
			i++;
		}
		choose_cpu_type(file, cpu_type);
		if ((handle_macho(file, ptr + offset)) < 0)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
