/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 15:53:01 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/21 13:14:03 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_textinfo						*get_text_info64(void *lc)
{
	struct segment_command_64	*seg_cmd64;
	struct section_64			*sec64;
	uint32_t					i;
	t_textinfo					*textinfo;

	if (!(textinfo = ft_memalloc(sizeof(t_textinfo))))
		return (NULL);
	seg_cmd64 = (struct segment_command_64 *)lc;
	sec64 = (void *)lc + sizeof(struct segment_command_64);
	i = 0;
	while (i < seg_cmd64->nsects)
	{
		if (ft_strcmp(SEG_TEXT, sec64->segname) == 0 \
			&& ft_strcmp(SECT_TEXT, sec64->sectname) == 0)
		{
			textinfo->offset = sec64->offset;
			textinfo->size = sec64->size;
			textinfo->start_offset = sec64->addr;
			return (textinfo);
		}
		sec64 = (void *)sec64 + sizeof(struct section_64);
		i++;
	}
	free(textinfo);
	return (NULL);
}

t_textinfo						*get_text_info(void *lc)
{
	struct segment_command		*seg_cmd;
	struct section				*sec;
	uint32_t					i;
	t_textinfo					*textinfo;

	if (!(textinfo = ft_memalloc(sizeof(t_textinfo))))
		return (NULL);
	seg_cmd = (struct segment_command *)lc;
	sec = (void *)lc + sizeof(struct segment_command);
	i = 0;
	while (i < seg_cmd->nsects)
	{
		if (ft_strcmp(SEG_TEXT, sec->segname) == 0 \
			&& ft_strcmp(SECT_TEXT, sec->sectname) == 0)
		{
			textinfo->offset = sec->offset;
			textinfo->size = sec->size;
			textinfo->start_offset = sec->addr;
			return (textinfo);
		}
		sec = (void *)sec + sizeof(struct section);
		i++;
	}
	free(textinfo);
	return (NULL);
}

int								search_text_segment64(t_filenm **file, \
											struct mach_header_64 *header64,\
											void *ptr, uint32_t i)
{
	struct load_command			*lc;
	t_textinfo					*textinfo;

	lc = (void *)ptr + sizeof(struct mach_header_64);
	while (i < header64->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			textinfo = get_text_info64(lc);
			if (textinfo != NULL)
			{
				if (!((*file)->text = ft_memalloc(textinfo->size)))
					return (EXIT_FAILURE);
				ft_memcpy((*file)->text, \
						(void *)ptr + textinfo->offset, textinfo->size);
				(*file)->text_start_offset = textinfo->start_offset;
				(*file)->text_size = textinfo->size;
				free(textinfo);
				break ;
			}
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	return (EXIT_SUCCESS);
}

static int						search_text_segment(t_filenm **file, \
											struct mach_header *header,\
											void *ptr, uint32_t i)
{
	struct load_command			*lc;
	t_textinfo					*textinfo;

	lc = (void *)ptr + sizeof(struct mach_header);
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			textinfo = get_text_info(lc);
			if (textinfo != NULL)
			{
				if (!((*file)->text = ft_memalloc(textinfo->size)))
					return (EXIT_FAILURE);
				ft_memcpy((*file)->text, \
						(void *)ptr + textinfo->offset, textinfo->size);
				(*file)->text_start_offset = textinfo->start_offset;
				(*file)->text_size = textinfo->size;
				free(textinfo);
				break ;
			}
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	return (EXIT_SUCCESS);
}

int								get_text(t_filenm **file, void *ptr,\
									struct mach_header_64 *header64, \
									struct mach_header *header)
{
	if (header == NULL)
	{
		if ((search_text_segment64(file, header64, ptr, 0)) < 0)
			return (EXIT_FAILURE);
	}
	else
	{
		if ((search_text_segment(file, header, ptr, 0)) < 0)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
