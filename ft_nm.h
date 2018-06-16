/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:21:10 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/16 14:30:17 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef FT_NM_H
# define FT_NM_H

#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "./libft/libft.h"

# define 				IS_64		8
# define				IS_AR		4
# define				IS_FAT		2
# define				is_SWAP 	1

typedef struct          s_symbol
{
	char				*name;
	char				type;
	uint64_t			value;
	struct s_symbol		*next;
}						t_symbol;

typedef struct			s_filenm
{
	char				*filename;
	int					type_flag;
	char				*err_msg;
	t_symbol			sym;
}						t_filenm;


#endif