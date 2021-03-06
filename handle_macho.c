/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 18:57:41 by ryaoi             #+#    #+#             */
/*   Updated: 2018/07/01 14:53:28 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void						search_section_index64(t_secindex **secindex,\
														void *lc)
{
	struct segment_command_64	*seg_cmd64;
	struct section_64			*sec64;
	uint32_t					i;

	seg_cmd64 = (struct segment_command_64 *)lc;
	sec64 = (void *)lc + sizeof(struct segment_command_64);
	i = 0;
	while (i < seg_cmd64->nsects)
	{
		if (ft_strcmp(SEG_TEXT, sec64->segname) == 0)
		{
			if (ft_strcmp(SECT_TEXT, sec64->sectname) == 0)
				(*secindex)->text_text = (*secindex)->index;
		}
		else if (ft_strcmp(SEG_DATA, sec64->segname) == 0)
		{
			if (ft_strcmp(SECT_DATA, sec64->sectname) == 0)
				(*secindex)->data_data = (*secindex)->index;
			if (ft_strcmp(SECT_BSS, sec64->sectname) == 0)
				(*secindex)->data_bss = (*secindex)->index;
		}
		(*secindex)->index += 1;
		sec64 = (void *)sec64 + sizeof(struct section_64);
		i++;
	}
}

static void						search_section_index(t_secindex **secindex, \
													void *lc)
{
	struct segment_command		*seg_cmd;
	struct section				*sec;
	uint32_t					i;

	seg_cmd = (struct segment_command *)lc;
	sec = (void *)lc + sizeof(struct segment_command);
	i = 0;
	while (i < seg_cmd->nsects)
	{
		if (ft_strcmp(SEG_TEXT, sec->segname) == 0)
		{
			if (ft_strcmp(SECT_TEXT, sec->sectname) == 0)
				(*secindex)->text_text = (*secindex)->index;
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

static int						get_secindex64(t_filenm **file,\
											t_secindex **secindex, \
											struct mach_header_64 *header64,\
											void *ptr)
{
	uint32_t					i;
	struct load_command			*lc;

	lc = (void *)ptr + sizeof(struct mach_header_64);
	(*file)->filesize -= sizeof(struct mach_header_64);
	i = 0;
	while (i < header64->ncmds && (*file)->filesize >= lc->cmdsize)
	{
		if (lc->cmd == LC_SEGMENT_64)
			search_section_index64(secindex, lc);
		if (lc->cmd == LC_SYMTAB)
			(*secindex)->symtab_value = (void *)lc;
		(*file)->filesize -= lc->cmdsize;
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	if (i != header64->ncmds || (*secindex)->symtab_value == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int						get_secindex(t_filenm **file,\
											t_secindex **secindex, \
											struct mach_header *header,\
											void *ptr)
{
	uint32_t					i;
	struct load_command			*lc;

	lc = (void *)ptr + sizeof(struct mach_header);
	(*file)->filesize -= sizeof(struct mach_header);
	i = 0;
	while (i < header->ncmds && (*file)->filesize >= lc->cmdsize)
	{
		if (lc->cmd == LC_SEGMENT)
			search_section_index(secindex, lc);
		if (lc->cmd == LC_SYMTAB)
			(*secindex)->symtab_value = (void *)lc;
		(*file)->filesize -= lc->cmdsize;
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	if (i != header->ncmds || (*secindex)->symtab_value == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int								handle_macho(t_filenm **file, void *ptr)
{
	struct mach_header_64		*header64;
	struct mach_header			*header;

	header64 = NULL;
	header = NULL;
	if (init_secindex(&((*file)->secindex)) < 0)
		return (EXIT_FAILURE);
	if (((*file)->type_flag & IS_64) > 1)
	{
		header64 = (struct mach_header_64 *)ptr;
		if ((get_secindex64(file, &((*file)->secindex), header64, ptr))\
			== EXIT_FAILURE)
			return (corrupt_msg(file));
	}
	else
	{
		header = (struct mach_header *)ptr;
		if ((get_secindex(file, &((*file)->secindex), header, ptr))\
			== EXIT_FAILURE)
			return (corrupt_msg(file));
	}
	if (((get_symbol_and_text(file, ptr, header64, header))) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
