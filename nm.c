/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:28:11 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/16 21:16:29 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"



int				handle_arch(t_filenm **file, void *ptr)
{
	long			magic_number;

	ft_printf("handle_arch\n");
	magic_number = *(long *)ptr;
	if ((int)magic_number == MH_MAGIC_64 ||  (int)magic_number == MH_CIGAM_64)
		(*file)->type_flag += IS_64;
	if (magic_number == MH_AR_64)
		(*file)->type_flag += IS_AR;
	if ((int)magic_number == MH_CIGAM_64 || (int)magic_number == MH_CIGAM\
		|| (int)magic_number == FAT_CIGAM)
		(*file)->type_flag += IS_SWAP;
	if ((int)magic_number == FAT_MAGIC)
		(*file)->type_flag = IS_FAT;
	// if (((*file)->type_flag & IS_FAT))
	// 	handle_fat(file, ptr);
	// if (((*file)->type_flag & IS_AR))
	// 	handle_ar(file, ptr);
	// else
	handle_macho(file, ptr);
	return (EXIT_SUCCESS);
}

int				handle_file(t_filenm **ptr)
{
    int         	fd;
	void			*mmap_ptr;
	struct stat		buf;

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
		(*ptr)->err_msg = ft_strdup("mmap failed\n");
	else
	handle_arch(ptr, mmap_ptr);
	if ((munmap(mmap_ptr, buf.st_size))< 0)
		(*ptr)->err_msg = ft_strdup("munmap failed\n");
	return (EXIT_SUCCESS);
}

int     get_file(t_filenm **file, int argc, char **argv)
{
    int         i;
    t_filenm    *ptr;
    if (argc < 2)
    {
        if ((ptr = add_filenm(file, "a.out")) == NULL)
			return (EXIT_FAILURE);
        handle_file(&ptr);
    }
    else
    {
        i = 1;
        while(i < argc)
        {
            if ((ptr = add_filenm(file, argv[i])) == NULL)
				return (EXIT_FAILURE);
            handle_file(&ptr);
            i++;
        }
    }
    return(EXIT_SUCCESS);
}

int     main(int argc, char **argv)
{
    t_filenm    *file;
    t_filenm    *ptr_file;
    
    file = NULL;
    if ((get_file(&file, argc, argv)) < 0)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}