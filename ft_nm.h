/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:21:10 by ryaoi             #+#    #+#             */
/*   Updated: 2018/07/01 18:20:19 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>
# include "./libft/libft.h"

# define WAS_FAT 64
# define IS_POWERPC 32
# define IS_OTOOL 16
# define IS_64 8
# define IS_AR 4
# define IS_FAT 2
# define ERR_OPEN_PERM "Permission denied\n"
# define ERR_OPEN_NOSUCH "No such file or directory.\n"
# define ERR_FSTAT "fstat failed\n"
# define ERR_MMAP_NM "The file was not recognized as a valid object file\n"
# define ERR_MMAP_OTOOL " is not an object file\n"
# define ERR_MUNMAP "munmap failed\n"
# define ERR_MSG_CORRUPT "corrupted file\n"
# define ARCH_X86_64_NM " (architecture x86_64)"
# define ARCH_X86_64_OTOOL " (for architecture x86_64)"
# define ARCH_I386_NM " (architecture i386)"
# define ARCH_I386_OTOOL " (for architecture i386)"
# define ARCH_PPC64_NM " (architecture ppc64)"
# define ARCH_PPC64_OTOOL " (for architecture ppc64)"
# define ARCH_PPC_NM " (architecture ppc)"
# define ARCH_PPC_OTOOL " (for architecture ppc)"
# define MH_AR_64 0x213C617263683E0A
# define MH_RA_64 0x0a3e686372613c21

typedef struct			s_symbol
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
	void				*text;
	uint32_t			text_size;
	uint64_t			text_start_offset;
	int64_t				filesize;
	int64_t				real_filesize;
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

typedef struct			s_textinfo
{
	int					offset;
	int					size;
	uint64_t			start_offset;
}						t_textinfo;

typedef struct			s_arch
{
	uint32_t			intel64_offset;
	uint32_t			intel64_size;
	uint32_t			intel32_offset;
	uint32_t			intel32_size;
	uint32_t			ppc64_offset;
	uint32_t			ppc64_size;
	uint32_t			ppc32_offset;
	uint32_t			ppc32_size;
}						t_arch;

t_filenm				*add_filenm(t_filenm **head, char *name, int is_otool);
int						count_filenm(t_filenm *file);
int						init_secindex(t_secindex **head);
int						handle_arch(t_filenm **file, void *ptr);
int						handle_macho(t_filenm **file, void *ptr);
int						get_symbol_and_text(t_filenm **file, void *ptr,\
								struct mach_header_64 *header64,\
								struct mach_header *header);
int						handle_powerpc(t_filenm **file, void *ptr);
int						add_symbol(t_filenm **head, char *symname, \
								struct nlist nlist);
int						add_symbol64(t_filenm **head, char *symname, \
								struct nlist_64 nlist64);
int						get_symbol(t_filenm **file, t_secindex *secindex,\
								void *ptr);
int						get_symbol_sub(t_filenm **file, void *ptr,\
									struct nlist_64 nlist64,\
									struct nlist nlist32);
int						get_symbolswap(t_filenm **file, \
							t_secindex *secindex, void *ptr);
int						sort_symbol(t_symbol **sym);
void					free_filenm(t_filenm **head);
int						handle_file(t_filenm **ptr);
int						handle_fat(t_filenm **file, void *ptr,\
								struct fat_header *fatheader);
int						handle_multiple_arch(t_filenm **file, void *ptr,\
								t_arch arch);
int						handle_ar(t_filenm **file, void *ptr,\
								t_filenm *file_ar, t_obj_header *ar_header);
int						print_symbol(int total_filenm, t_filenm *file);
void					print_text(t_filenm *file);
int						get_text(t_filenm **file, void *ptr,\
				struct mach_header_64 *header64, struct mach_header *header);
int						copy_text_section(t_filenm **file, void *ptr, \
									t_textinfo *textinfo);
int						get_textswap(t_filenm **file, void *ptr,\
						struct mach_header *header);
int						corrupt_msg(t_filenm **file);
uint32_t				swap32(u_int32_t origin);
uint16_t				swap16(u_int16_t origin);

#endif
