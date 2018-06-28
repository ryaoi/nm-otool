/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/20 14:08:21 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/28 13:49:58 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		print_symbolinfo(t_filenm *file, t_symbol *ptr)
{
	if (file->type_flag & IS_64 && ptr->type != 'U')
		ft_printf("%016lx", ptr->value);
	else if (file->type_flag & IS_64 && ptr->type == 'U')
		ft_printf("% 16c", ' ');
	else if (ptr->type != 'U')
		ft_printf("%08lx", ptr->value);
	else
		ft_printf("% 8c", ' ');
	ft_printf(" %c %s\n", ptr->type, ptr->name);
}

static void		print_err_msg(char *filename, char *err_msg)
{
	ft_putstr_fd("ft_nm: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(err_msg, 2);
}

int				print_symbol(int total_filenm, t_filenm *file)
{
	t_symbol	*ptr;

	while (file != NULL)
	{
		if (file->err_msg != NULL)
			print_err_msg(file->filename, file->err_msg);
		if (!(file->type_flag & WAS_FAT)\
			&& file->sym != NULL\
			&& total_filenm > 1 && file->err_msg == NULL)
			ft_printf("\n%s:\n", file->filename);
		ptr = file->sym;
		while (ptr != NULL)
		{
			if (ptr->type == '*')
			{
				ptr = ptr->next;
				continue ;
			}
			print_symbolinfo(file, ptr);
			ptr = ptr->next;
		}
		file = file->next;
	}
	return (EXIT_SUCCESS);
}
