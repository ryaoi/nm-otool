/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol_sub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/01 15:00:15 by ryaoi             #+#    #+#             */
/*   Updated: 2018/07/01 15:04:14 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int					get_symbol_sub(t_filenm **file, void *ptr,\
								 	struct nlist_64 nlist64,\
									struct nlist nlist32)
{
	char					*stringtable;
	struct symtab_command	*sym;

	sym = (struct symtab_command *)(*file)->secindex->symtab_value;
	stringtable = (void *)ptr + sym->stroff;
	if ((*file)->type_flag & IS_64)
	{
		if ((int64_t)(stringtable + \
			nlist64.n_un.n_strx) < (int64_t)ptr + (*file)->real_filesize)
		add_symbol64(file, stringtable + nlist64.n_un.n_strx, \
			nlist64);
		else
			return (EXIT_FAILURE);
	}
	else
	{
		if ((int64_t)(stringtable + \
			nlist32.n_un.n_strx)  < (int64_t)ptr + (*file)->real_filesize)
		add_symbol(file, stringtable + nlist32.n_un.n_strx, \
			nlist32);
		else
			return (EXIT_FAILURE);
	}
	(*file)->filesize -= (int)sizeof(struct nlist);
	return (EXIT_SUCCESS);
}