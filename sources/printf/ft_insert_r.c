/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insert_r.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 02:04:57 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/20 09:45:26 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int					put_r(unsigned char c, t_frmt_fs *f)
{
	char			**a;
	int				len;
	int				tmp;
	const char		*alph[68] = {
		"nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs", "ht",\
		"nl", "vt", "np", "cr", "so", "si", "dle", "dc1", "dc2", "dc3", "dc4",\
		"nak", "syn", "etb", "can", "em", "sub", "esc", "fs", "gs", "rs",\
		"us", "sp", "del",\
		"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "HT",\
		"NL", "VT", "NP", "CR", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4",\
		"NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GC", "RS",\
		"US", "SP", "DEL" };

	c > 32 ? c = 33 : 0;
	len = 0;
	a = (char **)(f->spec == 'r' ? alph : alph + 34);
	if (!f->sharp)
		len += write(STDOUT_FILENO, "\x1b[7m", 4);
	tmp = ft_strlen(a[c]);
	len += write(STDOUT_FILENO, "<", 1);
	len += write(STDOUT_FILENO, a[c], tmp);
	len += write(STDOUT_FILENO, ">", 1);
	if (!f->sharp)
		len += write(STDOUT_FILENO, "\x1b[0m", 4);
	return (0);
}

ssize_t				ft_insert_r(t_frmt_fs *f, va_list arg)
{
	const unsigned char		*npstr = va_arg(arg, unsigned char *);
	ssize_t					len;
	int						scan;

	len = 0;
	if (f->ispre == 0)
		f->precision = ft_strlen((char *)npstr);
	if (!f->orient)
		len += ft_putchar_n(' ', f->field_len - f->precision);
	scan = -1;
	while (++scan < f->precision)
	{
		if (npstr[scan] <= 32 || npstr[scan] == 127)
			len += put_r(npstr[scan], f);
		else
			len += write(STDOUT_FILENO, npstr + scan, 1);
	}
	if (f->orient)
		len += ft_putchar_n(' ', f->field_len - f->precision);
	return (len);
}
