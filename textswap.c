/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textswap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 21:19:06 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/24 22:11:10 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_textinfo				*get_text_info(void *lc)
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
	while (i < swap32(seg_cmd->nsects))
	{
		if (ft_strcmp(SEG_TEXT, sec->segname) == 0 \
			&& ft_strcmp(SECT_TEXT, sec->sectname) == 0)
		{
			textinfo->offset = swap32(sec->offset);
			textinfo->size = swap32(sec->size);
			textinfo->start_offset = swap32(sec->addr);
			return (textinfo);
		}
		sec = (void *)sec + sizeof(struct section);
		i++;
	}
	free(textinfo);
	return (NULL);
}

static int						search_text_segment(t_filenm **file, \
											struct mach_header *header,\
											void *ptr, uint32_t i)
{
	struct load_command			*lc;
	t_textinfo					*textinfo;

	lc = (void *)ptr + sizeof(struct mach_header);
	while (i < swap32(header->ncmds))
	{
		if (swap32(lc->cmd) == LC_SEGMENT)
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
		lc = (void *)lc + swap32(lc->cmdsize);
		i++;
	}
	return (EXIT_SUCCESS);
}

int								get_textswap(t_filenm **file, void *ptr,\
									struct mach_header *header)
{
	if ((search_text_segment(file, header, ptr, 0)) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
