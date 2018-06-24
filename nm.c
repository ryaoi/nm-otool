/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:28:11 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/22 15:57:13 by ryaoi            ###   ########.fr       */
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
		if ((ptr = add_filenm(file, "a.out", 0)) == NULL)
			return (EXIT_FAILURE);
		handle_file(&ptr);
	}
	else
	{
		i = 1;
		while (i < argc)
		{
			if ((ptr = add_filenm(file, argv[i], 0)) == NULL)
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
	return (EXIT_SUCCESS);
}
