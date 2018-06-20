/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:28:11 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/20 15:25:47 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			exec_handler(t_filenm **file, void *ptr, long magic_number)
{
	if (((*file)->type_flag & IS_FAT))
	{
		if ((handle_fat(file, ptr)) < 0)
			return (EXIT_FAILURE);
	}
	else if (((*file)->type_flag & IS_AR))
	{
		if ((handle_ar(file, ptr, NULL)) < 0)
			return (EXIT_FAILURE);
	}
	else if (((*file)->type_flag & IS_64) || (uint32_t)magic_number == MH_MAGIC\
			|| (uint32_t)magic_number == MH_CIGAM)
	{
		if ((handle_macho(file, ptr)) < 0)
			return (EXIT_FAILURE);
	}
	else
		(*file)->err_msg = \
			ft_strdup("The file was not recognized as a valid object file\n");
	return (EXIT_SUCCESS);
}

int					handle_arch(t_filenm **file, void *ptr)
{
	long			magic_number;

	magic_number = *(long *)ptr;
	if ((uint32_t)magic_number == MH_MAGIC_64 \
		|| (uint32_t)magic_number == MH_CIGAM_64\
		|| (uint32_t)magic_number == FAT_MAGIC_64 \
		|| (uint32_t)magic_number == FAT_CIGAM_64)
		(*file)->type_flag += IS_64;
	if (magic_number == (long)MH_AR_64 || magic_number == (long)MH_RA_64)
		(*file)->type_flag += IS_AR;
	if ((uint32_t)magic_number == MH_CIGAM_64 \
		|| (uint32_t)magic_number == MH_CIGAM\
		|| (uint32_t)magic_number == FAT_CIGAM\
		|| (uint32_t)magic_number == FAT_CIGAM_64)
		(*file)->type_flag += IS_SWAP;
	if ((uint32_t)magic_number == FAT_MAGIC\
		|| (uint32_t)magic_number == FAT_CIGAM
		|| (uint32_t)magic_number == FAT_MAGIC_64\
		|| (uint32_t)magic_number == FAT_CIGAM_64)
		(*file)->type_flag = IS_FAT;
	if (exec_handler(file, ptr, magic_number) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

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
		(*ptr)->err_msg = ft_strdup("mmap failed\n");
	else
		handle_arch(ptr, mmap_ptr);
	if ((munmap(mmap_ptr, buf.st_size)) < 0)
		(*ptr)->err_msg = ft_strdup("munmap failed\n");
	return (EXIT_SUCCESS);
}

int					get_file(t_filenm **file, int argc, char **argv)
{
	int				i;
	t_filenm		*ptr;

	ptr = NULL;
	if (argc < 2)
	{
		if ((ptr = add_filenm(file, "a.out")) == NULL)
			return (EXIT_FAILURE);
		handle_file(&ptr);
	}
	else
	{
		i = 1;
		while (i < argc)
		{
			if ((ptr = add_filenm(file, argv[i])) == NULL)
				return (EXIT_FAILURE);
			handle_file(&ptr);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

int					main(int argc, char **argv)
{
	t_filenm		*file;

	file = NULL;
	if ((get_file(&file, argc, argv)) < 0)
		return (EXIT_FAILURE);
	print_symbol(count_filenm(file), file);
	free_filenm(&file);
	while(1);
	return (EXIT_SUCCESS);
}
