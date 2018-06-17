/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swapbyte.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/17 20:03:28 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/17 21:46:59 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

uint16_t		swap16(u_int16_t origin)
{
	return ((origin>>8) | (origin<<8));
}

uint32_t		swap32(u_int32_t origin)
{
	return (((origin >> 24) & 0xff)
				| ((origin>>8)&0xff00)
				| ((origin<<8)&0xff0000)
				| ((origin<<24)&0xff000000));
}


uint64_t		swap64(u_int64_t origin)
{
	uint64_t x;
	
	x = origin;
	x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
	x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
	x = (x & 0x00FF00FF00FF00FF) << 8  | (x & 0xFF00FF00FF00FF00) >> 8;
	return (x);
}