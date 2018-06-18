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

static char			*lib_and_objname(char **filename, char *objname)
{
	char			*tmp1;
	char			*tmp2;
	char			ret;

	if (!(tmp1 = ft_strjoin("(", *filename)))
		return (NULL);
	if (!(tmp2 = ft_strjoin(objname, ")")))
		return (NULL);
	//check if this free well
	ret = ft_strjoinfree(tmp1, tmp2);
	return (ret);
}

int					handle_ar(t_filenm **file, void *ptr)
{
	t_filenm		*file_ar;
	t_obj_header	*ar_header;
	int				offset;
	char			*name;
	void			*maxiter;

	ar_header = (void *)ptr +  sizeof(MH_AR_64);
	ft_printf("sizeof(MH_AR_64):%d\n", sizeof(MH_AR_64));
	offset = atoi(ar_header->size);
	ft_printf("offset:%d\n", offset);
	maxiter = ar_header + sizeof(t_obj_header);
	ft_printf("symbol table addr:%x\n", sizeof(MH_AR_64) + sizeof(t_obj_header) + 4);
	ft_printf("iter:%x swapiter:%x\n", *(int *)maxiter, swap32(*(int *)maxiter));
	ar_header = ar_header->longname + offset;
	ft_printf("name:%s\n", ar_header->longname);
	return (EXIT_SUCCESS);
}
