/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 18:59:16 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/25 18:31:28 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			count_symtable(void *ptr, int iter, int ret)
{
	int				stock[2048];
	int				i;
	int				num;

	ft_bzero(stock, 2048 * sizeof(int));
	ret = 0;
	while (iter)
	{
		num = *(int *)ptr;
		i = 0;
		while (stock[i] != 0)
		{
			if (stock[i] == num)
				break ;
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

int					handle_ar(t_filenm **file, void *ptr, t_filenm *file_ar)
{
	t_filenm		*file_ptr;
	t_obj_header	*ar_header;
	char			*name;
	int				iter;

	ar_header = (void *)ptr + sizeof(MH_AR_64);
	iter = count_symtable((void *)ar_header + sizeof(t_obj_header) + 28, \
		(*(int *)((void *)ar_header + sizeof(t_obj_header) + 20)) / 8, 0);
	ptr = (void *)ptr + sizeof(MH_AR_64) + sizeof(t_obj_header) + \
		ft_atoi(ar_header->size) + 20 + sizeof(t_obj_header);
	while (iter)
	{
		ar_header = (void *)ar_header + sizeof(t_obj_header) \
					+ ft_atoi(ar_header->size);
		name = lib_and_objname((*file)->filename, \
							(void *)ar_header + sizeof(t_obj_header));
		file_ptr = add_filenm(&file_ar, name, (*file)->type_flag & IS_OTOOL);
		free(name);
		handle_arch(&file_ptr, (void *)ptr + ft_atoi(ar_header->name + 3) - 20);
		ptr = (void *)ptr + ft_atoi(ar_header->size) + sizeof(t_obj_header);
		iter--;
	}
	link_file(file, &file_ar);
	return (EXIT_SUCCESS);
}
