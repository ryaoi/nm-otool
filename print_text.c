/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_text.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 16:49:38 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/24 22:32:52 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			print_ppc(t_filenm *file, uint32_t offset)
{
	uint32_t		i;
	uint32_t		align;
	void			*ptr;

	align = 16;
	i = 0;
	while (i < align && (offset + i) < file->text_size)
	{
		ptr = (void *)file->text + offset + i;
		ft_printf("%08x", swap32((*(unsigned int *)ptr)));
		if (i+4 != align)
			ft_putstr(" ");
		else
			ft_putstr("\n");
		i += 4;
	}
	return (i);
}

static int			print_address(t_filenm *file, uint32_t offset)
{
	uint32_t		i;
	uint32_t		align;
	void			*ptr;

	i = 0;
	if (file->type_flag & IS_64)
		ft_printf("%016x", file->text_start_offset + offset);
	else
		ft_printf("%08x", file->text_start_offset + offset);
	ft_printf("        ");
	align = 16;
	if (ft_strstr(file->filename, "(architecture ppc)"))
		return (print_ppc(file, offset));
	while (i < align && (offset + i) < file->text_size)
	{
		ptr = (void *)file->text + offset + i;
		ft_printf("%02x", (*(unsigned char *)ptr));
		if (i < align - 1)
			ft_putstr(" ");
		else
			ft_putstr("\n");
		i++;
	}
	return (i);
}

void				print_text(t_filenm *file)
{
	uint32_t		offset;

	while (file != NULL)
	{
		if (file->err_msg != NULL)
		{
			ft_putstr_fd(file->filename, 2);
			ft_putstr_fd(":", 2);
			ft_putstr_fd(file->err_msg, 2);
		}
		else
		{
			ft_printf("%s:\nContents of (__TEXT,__text) section\n", \
					file->filename);
			offset = 0;
			while (offset < file->text_size)
				offset += print_address(file, offset);
			ft_printf("\n");
		}
		file = file->next;
	}
}
