/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 15:32:59 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/20 17:21:02 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			get_file(t_filenm **file, int argc, char **argv)
{
	int				i;
	t_filenm		*ptr;

	ptr = NULL;
	if (argc < 2)
	{
		ft_putstr_fd("Usage: ft_otool <object file>\n", 2);
		return (EXIT_FAILURE);
	}
	else
	{
		i = 1;
		while (i < argc)
		{
			if ((ptr = add_filenm(file, argv[i], IS_OTOOL)) == NULL)
				return (EXIT_FAILURE);
			handle_file(&ptr);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	t_filenm *file;

	file = NULL;
	if ((get_file(&file, argc, argv)) < 0)
		return (EXIT_FAILURE);
	print_text(file);
	free_filenm(&file);
	return (EXIT_SUCCESS);
}