/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 18:57:41 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/17 16:34:32 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"


int							search_section_index64(t_secindex **secindex, void *lc)
{
	struct segment_command_64	*seg_cmd64;
	struct section_64			*sec64;
	int							i;

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
	return (EXIT_SUCCESS);
}

int							search_section_index(t_secindex **secindex, void *lc)
{
	struct segment_command		*seg_cmd;
	struct section				*sec;
	int							i;

	seg_cmd = (struct segment_command *)lc;
	sec = (void *)lc + sizeof(struct segment_command);
	i = 0;
	while(i < seg_cmd->nsects)
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
	return (EXIT_SUCCESS);
}

int							get_secindex64(t_secindex **secindex, \
											struct mach_header_64 *header64,\
											void *ptr)
{
	int						i;
	struct load_command		*lc;

	lc = (void *)ptr + sizeof(struct mach_header_64);
	i = 0;
	while(i < header64->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			search_section_index64(secindex, lc);
		}
		if (lc->cmd == LC_SYMTAB)
			break;
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	(*secindex)->symtab_value = (void *)lc;
	return (EXIT_SUCCESS);
}

int							get_secindex(t_secindex **secindex, \
											struct mach_header *header,\
											void *ptr)
{
	int						i;
	struct load_command		*lc;

	lc = (void *)ptr + sizeof(struct mach_header);
	i = 0;
	while(i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			search_section_index(secindex, lc);
		}
		if (lc->cmd == LC_SYMTAB)
			break;
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	(*secindex)->symtab_value = (void *)lc;
	return (EXIT_SUCCESS);
}

int							handle_macho(t_filenm **file, void *ptr)
{
	struct mach_header_64	*header64;
	struct mach_header		*header;

	if (init_secindex(&((*file)->secindex)) < 0)
		return (EXIT_FAILURE);
	if (((*file)->type_flag & IS_64))
	{
		header64 = (struct mach_header_64 *)ptr;
		get_secindex64(&((*file)->secindex), header64, ptr);
		get_symbol(file, (*file)->secindex, ptr);
	}
	else
	{
		header = (struct mach_header *)ptr;
		get_secindex(&((*file)->secindex), header, ptr);
	// 		ft_printf("%i->__TEXT,__text\n", (*file)->secindex->text_text);
	// ft_printf("%i->__DATA,__data\n", (*file)->secindex->data_data);
	// ft_printf("%i->__DATA,__bss\n", (*file)->secindex->data_bss);
		get_symbol(file, (*file)->secindex, ptr);
	}
	return (EXIT_SUCCESS);
}