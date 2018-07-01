# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/14 18:49:54 by ryaoi             #+#    #+#              #
#    Updated: 2018/07/01 15:19:46 by ryaoi            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_NM		= 	ft_nm
NAME_OTOOL	= 	ft_otool

DIR_LIB	= libft

LIBFT	= $(DIR_LIB)/libft.a

SRCS_NM		= 	nm.c \
				filenm.c \
				print_symbol.c \
				secindex.c \
				sort_symbol.c \
				swapbyte.c \
				symbol.c \
				symbol_sub.c \
				handle_ar.c \
				handle_fat.c \
				handle_fat_sub.c \
				text.c \
				handle_macho.c \
				handle_macho_sub.c \
				handle_arch.c \
				handle_file.c \
				handle_powerpc.c \
				symbol_swap.c \
				textswap.c \
				corrupt_msg.c \
				copy_text_section.c

SRCS_OTOOL	= 	otool.c \
				filenm.c \
				text.c \
				handle_ar.c \
				handle_fat.c \
				handle_fat_sub.c \
				swapbyte.c \
				symbol.c \
				symbol_sub.c \
				handle_macho.c \
				handle_macho_sub.c \
				handle_arch.c \
				handle_file.c \
				secindex.c \
				sort_symbol.c \
				print_text.c \
				print_symbol.c \
				handle_powerpc.c \
				symbol_swap.c \
				textswap.c \
				corrupt_msg.c \
				copy_text_section.c

OBJ_NM		= $(SRCS_NM:.c=.o)
OBJ_OTOOL	= $(SRCS_OTOOL:.c=.o)

INCLUDE	= -I./ft_nm.h \
		  -I./libft/libft.h

HEADER  = ./ft_nm.h \
		  ./libft/libft.h

CC		= gcc

CFLAGS	= -Wall -Wextra -Werror -g

all: $(NAME_NM) $(NAME_OTOOL)

%.o:%.c $(HEADER)
	$(CC) $(CFLAGS) -I./$(INCLUDE) -o $@ -c $< 

$(LIBFT):
	make -C $(DIR_LIB)

$(NAME_NM): $(LIBFT) $(OBJ_NM) $(HEADER) 
	$(CC) $(CFLAGS) -o $(NAME_NM) ./libft/libft.a $(OBJ_NM) $(INCLUDE) 

$(NAME_OTOOL): $(LIBFT) $(OBJ_OTOOL) $(HEADER) 
	$(CC) $(CFLAGS) -o $(NAME_OTOOL) ./libft/libft.a $(OBJ_OTOOL) $(INCLUDE) 

clean:
	make clean -C $(DIR_LIB)
	rm -rf $(OBJ_NM)
	rm -rf $(OBJ_OTOOL)

fclean: clean
	make fclean -C $(DIR_LIB)
	rm -rf $(NAME)

re: fclean all
