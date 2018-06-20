/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 16:47:44 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/20 19:03:42 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int					handle_file(t_filenm **ptr)
{
	int				fd;
	void			*mmap_ptr;
	struct stat		buf;

	mmap_ptr = NULL;
	if ((fd = open((*ptr)->filename, O_RDONLY)) < 0)
	{
		if (errno == EACCES)
			(*ptr)->err_msg = ft_strdup("Permission denied\n");
		if (errno == ENOENT)
			(*ptr)->err_msg = ft_strdup("No such file or directory.\n");
		return (EXIT_SUCCESS);
	}
	else if ((fstat(fd, &buf)) < 0)
		(*ptr)->err_msg = ft_strdup("fstat failed\n");
	else if ((mmap_ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))\
			== MAP_FAILED)
	{
		if ((*ptr)->type_flag & IS_OTOOL)
			(*ptr)->err_msg = \
				ft_strdup(" is not an object file\n");
		else
			(*ptr)->err_msg = \
			ft_strdup("The file was not recognized as a valid object file\n");	
	}
	else
		handle_arch(ptr, mmap_ptr);
	if ((mmap_ptr != MAP_FAILED) && ((munmap(mmap_ptr, buf.st_size)) < 0))
		(*ptr)->err_msg = ft_strdup("munmap failed\n");
	return (EXIT_SUCCESS);
}