/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryaoi <ryaoi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 14:28:11 by ryaoi             #+#    #+#             */
/*   Updated: 2018/06/16 14:40:42 by ryaoi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int     get_file(t_filenm **file, int argc, char **argv)
{
    int         i;

    if (argc < 2)
    {
        if ((add_file(file, "a.out")) < 0)
			return (EXIT_FAILURE);
    }
    else
    {
        i = 1;
        while(i < argc)
        {
            if ((add_file(file, argv[i])) < 0)
				return (EXIT_FAILURE);
            i++;
        }
    }
    return(EXIT_SUCCESS);
}

int     main(int argc, char **argv)
{
    t_filenm    *file;
    t_filenm    *ptr_file;
    
    file = NULL;
    if ((get_file(&file, argc, argv)) < 0)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}