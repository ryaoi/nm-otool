/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corrupt_msg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/01 13:59:54 by ryaoi             #+#    #+#             */
/*   Updated: 2018/07/01 14:57:03 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int				corrupt_msg(t_filenm **file)
{
	if ((*file)->err_msg != NULL)
		free((*file)->err_msg);
	(*file)->err_msg = ft_strdup(ERR_MMAP_NM);
	return (EXIT_FAILURE);
}
