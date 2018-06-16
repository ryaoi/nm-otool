/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 22:31:39 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/16 22:51:21 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int			get_symbol(t_filenm **file, t_secindex *secindex, void *ptr)
{
	struct symtab_command	*sym;
	int						i;
	char					*stringtable;
	struct nlist_64			*nlist64;
	struct nlist			*nlist;

	i = 0;
	ft_printf("get_symbol\n");
	sym = (struct symtab_command *)secindex->symtab_value;
	nlist64 = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	ft_printf("nsyms:%d\n", sym->nsyms);
	while(i < sym->nsyms)
	{
		if ((*file)->type_flag & IS_64)
			ft_printf("%s\n", stringtable + nlist64[i].n_un.n_strx);
		else
			ft_printf("%s\n", stringtable + nlist64[i].n_un.n_strx);
		i++;
	}
	return (EXIT_SUCCESS);
}