/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insert_s.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:22:03 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/20 09:45:26 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

int inline		ft_putchar_n(char c, ssize_t n)
{
	int		len;

	len = -1;
	while (++len < n)
	{
		write(STDOUT_FILENO, &c, 1);
	}
	return (len);
}

size_t			ft_print_str(t_frmt_fs *f, size_t len, size_t size, char *str)
{
	int		plen;

	plen = 0;
	if (!f->orient)
	{
		plen += ft_putchar_n(f->zerofill ? '0' : ' ',
		(unsigned)f->precision > len || !f->ispre \
		? size - len : size - f->precision);
		plen += write(STDOUT_FILENO, str, f->ispre && (unsigned)f->precision \
		< len ? f->precision : len);
	}
	else
	{
		plen += write(STDOUT_FILENO, str, f->ispre && (unsigned)f->precision \
		< len ? f->precision : len);
		plen += ft_putchar_n(f->zerofill ? '0' : ' ',
		(unsigned)f->precision > len || !f->ispre \
		? size - len : size - f->precision);
	}
	return (plen);
}

size_t			ft_insert_s(t_frmt_fs *f, va_list arg)
{
	char	*str;
	size_t	len;
	size_t	size;

	str = va_arg(arg, char*);
	size = f->field_len;
	if (f->ispre && !f->precision)
	{
		ft_putchar_n(f->zerofill ? '0' : ' ', size);
		return (size);
	}
	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	return (ft_print_str(f, len, size, str));
}
