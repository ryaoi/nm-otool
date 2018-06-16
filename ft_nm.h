/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:21:10 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/16 22:49:21 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef FT_NM_H
# define FT_NM_H

#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include "./libft/libft.h"

# define 				IS_64		8
# define				IS_AR		4
# define				IS_FAT		2
# define				IS_SWAP 	1

# define				MH_AR_64	0x213C617263683E0A

typedef struct          s_symbol
{
	char				*name;
	char				type;
	uint64_t			value;
	struct s_symbol		*next;
}						t_symbol;

typedef struct			s_secindex
{
	int					index;
	int					text_text;
	int					data_data;
	int					data_bss;
	void				*symtab_value;
}						t_secindex;

typedef struct			s_filenm
{
	char				*filename;
	int					type_flag;
	char				*err_msg;
	t_symbol			*sym;
	t_secindex			*secindex;
	struct s_filenm		*next;
}						t_filenm;

t_filenm				*add_filenm(t_filenm **head, char *name);
int						init_secindex(t_secindex **head);
int						handle_macho(t_filenm **file, void *ptr);
int						get_symbol(t_filenm **file, t_secindex *secindex, void *ptr);

#endif