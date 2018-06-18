/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/17 16:55:37 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/18 17:28:35 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int							handle_fat(t_filenm **file, void *ptr)
{
	struct fat_arch			*fatarch;
	struct fat_header		*fatheader;
	int						i;
	int						cpu_type;
	int						offset;

	i = 0;
	if ((*file)->type_flag == (IS_FAT))
	{
		fatheader = (struct fat_header *)ptr;
		fatarch = (void *)ptr + sizeof(struct fat_header);
		ft_printf("magic:%lx\n", fatheader->magic);
		ft_printf("nombre de arch:%ld\n", swap32(fatheader->nfat_arch));
		while (i < swap32(fatheader->nfat_arch))
		{
			if (swap32(fatarch->cputype) == CPU_TYPE_X86_64)
			{
				cpu_type = CPU_TYPE_X86_64;
				offset = swap32(fatarch->offset);
			}
			if (swap32(fatarch->cputype) == CPU_TYPE_I386 && cpu_type != CPU_TYPE_X86_64)
			{
				cpu_type = CPU_TYPE_I386;
				offset = swap32(fatarch->offset);
			}
			fatarch = (void *)fatarch + sizeof(struct fat_arch);
			i++;
		}
		if (cpu_type == CPU_TYPE_X86_64)
		{
			(*file)->type_flag -= IS_FAT;
			(*file)->type_flag += IS_64;
		}
		handle_macho(file, ptr + offset);
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
