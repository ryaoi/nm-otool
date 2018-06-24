/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_powerpc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 19:12:15 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/24 22:00:11 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"



static void						search_section_index(t_secindex **secindex, \
													void *lc)
{
	struct segment_command		*seg_cmd;
	struct section				*sec;
	uint32_t					i;

	seg_cmd = (struct segment_command *)lc;
	sec = (void *)lc + sizeof(struct segment_command);
	i = 0;
	while (i < swap32(seg_cmd->nsects))
	{
		if (ft_strcmp(SEG_TEXT, sec->segname) == 0)
		{
			if (ft_strcmp(SECT_TEXT, sec->sectname) == 0)
			{
				(*secindex)->text_text = (*secindex)->index;
			}
		}
		else if (ft_strcmp(SEG_DATA, sec->segname) == 0)
		{
			if (ft_strcmp(SECT_DATA, sec->sectname) == 0)
				(*secindex)->data_data = (*secindex)->index;
			if (ft_strcmp(SECT_BSS, sec->sectname) == 0)
				(*secindex)->data_bss = (*secindex)->index;
		}
		(*secindex)->index += 1;
		sec = (void *)sec + sizeof(struct section);
		i++;
	}
}

static int							get_secindex(t_secindex **secindex, \
											struct mach_header *header,\
											void *ptr)
{
	uint32_t					i;
	struct load_command			*lc;

	lc = (void *)ptr + sizeof(struct mach_header);
	i = 0;
	while (i < swap32(header->ncmds))
	{
		if (swap32(lc->cmd) == LC_SEGMENT)
		{
			search_section_index(secindex, lc);
		}
		if (swap32(lc->cmd) == LC_SYMTAB)
			break ;
		lc = (void *)lc + swap32(lc->cmdsize);
		i++;
	}
	(*secindex)->symtab_value = (void *)lc;
	return (EXIT_SUCCESS);
}

int								handle_powerpc(t_filenm **file, void* ptr)
{
	struct mach_header			*header;
	struct load_command			*lc;
	uint32_t					i;

	header = (struct mach_header *)ptr;
	lc = (void *)ptr + sizeof(struct mach_header);
	i = 0;
	if (init_secindex(&((*file)->secindex)) < 0)
		return (EXIT_FAILURE);
	header = (struct mach_header *)ptr;
	if ((*file)->type_flag & IS_OTOOL)
		return (get_textswap(file, ptr, header));
	get_secindex(&((*file)->secindex), header, ptr);
	get_symbolswap(file, (*file)->secindex, ptr);
	return (EXIT_SUCCESS);
}