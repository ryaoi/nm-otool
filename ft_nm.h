/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:21:10 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/18 19:16:23 by ryaoi            ###   ########.fr       */
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
# define				MH_RA_64	0x0a3e686372613c21

typedef struct          s_symbol
{
	char				*name;
	uint8_t				type;
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

typedef struct			s_obj_header
{
	char				name[16];
	char				tiemstamp[12];
	char				user_id[6];
	char				group_id[6];
	char				mode[8];
	char				size[8];
	char				end_header[4];
}						t_obj_header;

t_filenm				*add_filenm(t_filenm **head, char *name);
int						count_filenm(t_filenm *file);
int						init_secindex(t_secindex **head);
int						handle_arch(t_filenm **file, void *ptr);
int						handle_macho(t_filenm **file, void *ptr);
int						get_symbol(t_filenm **file, t_secindex *secindex, void *ptr);
int						sort_symbol(t_symbol **sym);
void					free_filenm(t_filenm **head);
int						handle_fat(t_filenm **file, void *ptr);
int						handle_ar(t_filenm **file, void *ptr);
int						print_symbol(int total_filenm, t_filenm *file);
uint64_t				swap64(u_int64_t origin);
uint32_t				swap32(u_int32_t origin);
uint16_t				swap16(u_int16_t origin);

#endif
