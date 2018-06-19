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
	t_filenm		*end_ar_save;

	if ((*file) == NULL)
	{
		(*file) = (*file_ar);
		return (EXIT_SUCCESS);
	}
	next_save = (*file)->next;
	(*file)->next = (*file_ar);
	end_ar_save = *file_ar;
	while (end_ar_save->next != NULL)
		end_ar_save = end_ar_save->next;
	end_ar_save = next_save;
	return (EXIT_SUCCESS);
}

int					handle_ar(t_filenm **file, void *ptr)
{
	t_filenm		*file_ar;
	t_filenm		*file_ptr;
	t_obj_header	*ar_header;
	int				offset;
	int				size_of_longname;
	char			*name;
	int				iter;

	file_ar = NULL;
	ar_header = (void *)ptr +  sizeof(MH_AR_64);
	ft_printf("sizeof(MH_AR_64):%d\n", sizeof(MH_AR_64));
	// maxiter = (void *)ar_header + sizeof(t_obj_header) + 4;
	// iter = *(int *)maxiter / 8;
	iter = count_symtable((void *)ar_header + sizeof(t_obj_header) + 28, (*(int *)((void *)ar_header + sizeof(t_obj_header) + 20))/ 8);
	ft_printf("iter:%d\n", iter);
	// ft_printf("symbol table addr:%x\n", sizeof(MH_AR_64) + sizeof(t_obj_header) + 4);
	// ft_printf("iter:%x swapiter:%x\n", *(int *)maxiter, swap32(*(int *)maxiter));
	ptr = (void *)ptr + sizeof(MH_AR_64) + sizeof(t_obj_header) + atoi(ar_header->size)  + 20 + sizeof(t_obj_header);
	ft_printf("jump to ptr:%x\n", sizeof(MH_AR_64) + sizeof(t_obj_header) + atoi(ar_header->size)  + 20 + sizeof(t_obj_header));
	// ft_printf("offset:%x\n", sizeof(MH_AR_64) + sizeof(t_obj_header) );
	while (iter)
	{
		offset = atoi(ar_header->size);
		// ft_printf("ptr:%d\n", sizeof(t_obj_header));
		// // ptr = (void *)ptr + size_of_longname;
		// ft_printf("ptr:%d\n", sizeof(t_obj_header) + size_of_longname);
		ft_printf("[name]:%s|offset:%d|skip:%d\n", (void *)ar_header + sizeof(t_obj_header), offset, size_of_longname);
		ar_header = (void *)ar_header + sizeof(t_obj_header) + offset;
		size_of_longname = atoi(ar_header->name + 3);
		if (size_of_longname > 20)
			size_of_longname -= 20;
		else
			size_of_longname = 0;
		name = lib_and_objname((*file)->filename, (void *)ar_header + sizeof(t_obj_header));
		file_ptr = add_filenm(&file_ar, name);
		handle_arch(&file_ptr, (void *)ptr + size_of_longname);
		// size_of_longname = atoi(ar_header->name + 4);
		// ft_printf("name:%ssizeoflongname:%d\n", ar_header->name, size_of_longname);
		ft_printf("next ptr is at: size:%d\t header_size:%d\t size_of_longname:%d\n", atoi(ar_header->size), sizeof(t_obj_header), size_of_longname);
		//ptr is good it doesnt need to check for the right offset
		ptr = (void *)ptr + atoi(ar_header->size) + sizeof(t_obj_header);
		iter--;
	}
	link_file(file, &file_ar);
	return (EXIT_SUCCESS);
}
