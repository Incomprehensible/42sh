/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insert_c.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 01:30:42 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/20 09:45:26 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

int			ft_insert_c(t_frmt_fs *f, va_list arg)
{
	char	c;
	size_t	size;

	size = f->field_len;
	c = va_arg(arg, int);
	if (!f->orient)
	{
		ft_putchar_n(f->zerofill ? '0' : ' ', size - 1);
		write(STDOUT_FILENO, &c, 1);
	}
	else
	{
		write(STDOUT_FILENO, &c, 1);
		ft_putchar_n(f->zerofill ? '0' : ' ', size - 1);
	}
	return (size ? size : 1);
}

int			ft_abc(int n)
{
	return (n < 0 ? n * -1 : n);
}
