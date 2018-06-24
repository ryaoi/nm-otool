/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 22:31:39 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/22 15:56:56 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static char					get_n_type_value(uint8_t n_type, uint64_t n_value)
{
	if ((n_type == (N_UNDF + N_EXT) || n_type == N_UNDF) && n_value == 0)
		return ('U');
	if (n_type == (N_UNDF + N_EXT))
		return ('C');
	if (n_type == N_UNDF)
		return ('c');
	else if (n_type == N_ABS + N_EXT)
		return ('A');
	else if (n_type == N_ABS)
		return ('a');
	else if (n_type == N_INDR)
		return ('I');
	else if (n_type == N_INDR + N_EXT)
		return ('I');
	return (0);
}

char						get_type(t_secindex *secindex, uint8_t n_type, \
								uint16_t n_sect, uint64_t n_value)
{
	char					ret_type;

	if ((ret_type = get_n_type_value(n_type, n_value)))
		return (ret_type);
	if ((n_type & N_STAB))
		return ('*');
	if (n_sect == secindex->text_text && (n_type & N_EXT))
		return ('T');
	else if (n_sect == secindex->text_text)
		return ('t');
	else if (n_sect == secindex->data_data && (n_type & N_EXT))
		return ('D');
	else if (n_sect == secindex->data_data)
		return ('d');
	else if (n_sect == secindex->data_bss && (n_type & N_EXT))
		return ('B');
	else if (n_sect == secindex->data_bss)
		return ('b');
	else if (n_type & N_EXT)
		return ('S');
	else
		return ('s');
}

int							add_symbol64(t_filenm **head, char *symname, \
								struct nlist_64 nlist64)
{
	t_symbol				*new;
	t_symbol				*ptr;

	if (!(new = malloc(sizeof(t_symbol))))
		return (EXIT_FAILURE);
	if (!(new->name = ft_strdup(symname)))
		return (EXIT_FAILURE);
	new->type = get_type((*head)->secindex, nlist64.n_type, \
						nlist64.n_sect, nlist64.n_value);
	new->value = nlist64.n_value;
	new->next = NULL;
	if ((*head)->sym == NULL)
		(*head)->sym = new;
	else
	{
		ptr = (*head)->sym;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
	}
	return (EXIT_SUCCESS);
}

int							add_symbol(t_filenm **head, char *symname, \
								struct nlist nlist)
{
	t_symbol				*new;
	t_symbol				*ptr;

	if (!(new = malloc(sizeof(t_symbol))))
		return (EXIT_FAILURE);
	if (!(new->name = ft_strdup(symname)))
		return (EXIT_FAILURE);
	new->type = get_type((*head)->secindex, nlist.n_type, \
						nlist.n_sect, nlist.n_value);
	new->value = nlist.n_value;
	new->next = NULL;
	if ((*head)->sym == NULL)
		(*head)->sym = new;
	else
	{
		ptr = (*head)->sym;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
	}
	return (EXIT_SUCCESS);
}

int							get_symbol(t_filenm **file, \
										t_secindex *secindex, void *ptr)
{
	struct symtab_command	*sym;
	uint32_t				i;
	char					*stringtable;
	struct nlist_64			*nlist64;
	struct nlist			*nlist32;

	i = 0;
	sym = (struct symtab_command *)secindex->symtab_value;
	nlist64 = (void *)ptr + sym->symoff;
	nlist32 = (void *)ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;
	while (i < sym->nsyms)
	{
		if ((*file)->type_flag & IS_64)
			add_symbol64(file, stringtable + nlist64[i].n_un.n_strx, \
			nlist64[i]);
		else
		{
			add_symbol(file, stringtable + nlist32[i].n_un.n_strx, \
			nlist32[i]);
		}
		i++;
	}
	sort_symbol(&((*file)->sym));
	return (EXIT_SUCCESS);
}
