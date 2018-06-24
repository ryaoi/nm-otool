/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 16:47:44 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/21 14:41:06 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			open_failed(t_filenm **ptr)
{
	if (errno == EACCES)
		(*ptr)->err_msg = ft_strdup(ERR_OPEN_PERM);
	if (errno == ENOENT)
		(*ptr)->err_msg = ft_strdup(ERR_OPEN_NOSUCH);
	return (EXIT_SUCCESS);
}

int					handle_file(t_filenm **ptr)
{
	int				fd;
	void			*mmap_ptr;
	struct stat		buf;

	mmap_ptr = NULL;
	if ((fd = open((*ptr)->filename, O_RDONLY)) < 0)
		return (open_failed(ptr));
	else if ((fstat(fd, &buf)) < 0)
		(*ptr)->err_msg = ft_strdup(ERR_FSTAT);
	else if ((mmap_ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))\
			== MAP_FAILED)
	{
		if ((*ptr)->type_flag & IS_OTOOL)
			(*ptr)->err_msg = \
				ft_strdup(ERR_MMAP_OTOOL);
		else
			(*ptr)->err_msg = \
			ft_strdup(ERR_MMAP_NM);
	}
	else
		handle_arch(ptr, mmap_ptr);
	if ((mmap_ptr != MAP_FAILED) && ((munmap(mmap_ptr, buf.st_size)) < 0))
		(*ptr)->err_msg = ft_strdup(ERR_MUNMAP);
	return (EXIT_SUCCESS);
}
