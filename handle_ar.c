/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 18:59:16 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/18 20:07:30 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			count_symtable(void *ptr, int iter)
{
	int				stock[2048];
	int				ret;
	int				i;
	int				*num;

	ft_bzero(stock, 2048*sizeof(int));
	ret = 0;
	while(iter)
	{
		num = * (int *)ptr;
		i = 0;
		while(stock[i] != 0)
		{
			if (stock[i] == num)
				break;
			i++;
		}
		if (stock[i] == 0)
		{
			stock[i] = num;
			ret++;
		}
		ptr += 8;
		iter--;
	}
	return (ret);
}

static char			*lib_and_objname(char *filename, char *objname)
{
	char			*tmp1;
	char			*tmp2;
	char			*ret;

	if (!(tmp1 = ft_strjoin("(", objname)))
		return (NULL);

	if (!(tmp2 = ft_strjoini(tmp1, ")", 1)))
		return (NULL);
	//check if this free well
	ret = ft_strjoini(filename, tmp2, 2);
	return (ret);
}

int					link_file(t_filenm **file, t_filenm **file_ar)
{
	t_filenm		*next_save;

	next_save = (*file)->next;
	(*file)->next = (*file_ar);
	(*file_ar)->next = next_save;
	return (EXIT_SUCCESS);
}

int					handle_ar(t_filenm **file, void *ptr)
{
	t_filenm		*file_ar;
	t_filenm		*file_ptr;
	t_obj_header	*ar_header;
	int				offset;
	char			*name;
	int				iter;

	file_ar = NULL;
	ar_header = (void *)ptr +  sizeof(MH_AR_64);
	ft_printf("sizeof(MH_AR_64):%d\n", sizeof(MH_AR_64));
	// maxiter = (void *)ar_header + sizeof(t_obj_header) + 4;
	// iter = *(int *)maxiter / 8;
	iter = count_symtable((void *)ar_header + sizeof(t_obj_header) + 12, (*(int *)((void *)ar_header + sizeof(t_obj_header) + 4))/ 8);
	ft_printf("iter:%d\n", iter);
	// ft_printf("symbol table addr:%x\n", sizeof(MH_AR_64) + sizeof(t_obj_header) + 4);
	// ft_printf("iter:%x swapiter:%x\n", *(int *)maxiter, swap32(*(int *)maxiter));
	ptr = (void *)ptr + sizeof(MH_AR_64) + sizeof(t_obj_header) + atoi(ar_header->size) + sizeof(t_obj_header) - 12;
	ft_printf("jump to ptr:%x\n", sizeof(MH_AR_64) + sizeof(t_obj_header) + atoi(ar_header->size) + sizeof(t_obj_header) - 12);
	ft_printf("offset:%x\n", sizeof(MH_AR_64) + sizeof(t_obj_header));
	while (iter)
	{	
		offset = atoi(ar_header->size);
		ar_header = (void *)ar_header->longname + offset;
		name = lib_and_objname((*file)->filename, ar_header->longname);
		ft_printf("new name:%s\n", name);
		ft_printf("size:%d\n", atoi(ar_header->size));
		file_ptr = add_filenm(&file_ar, name);
		handle_arch(&file_ptr, ptr);
		ptr = (void *)ptr + atoi(ar_header->size) + 60;
		iter--;
	}
	link_file(file, &file_ar);
	return (EXIT_SUCCESS);
}
