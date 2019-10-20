/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 03:10:55 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/20 09:43:47 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

char	*ln_n2s_rformat(t_8b a, const int len,\
const char filler, char *st)
{
	int		ind;

	if (st)
	{
		ind = len;
		ft_memset(st, filler, len);
		st[ind] = 0;
		if (!a)
			st[ind - 1] = '0';
		while (a && --ind > -1)
		{
			st[ind] = (a % 10) + 48;
			a /= 10;
		}
		st[len] = 0;
	}
	return (st + len);
}

int		ft_printf(const char *format, ...)
{
	va_list				arg;
	size_t				n[2];
	char				*inputer[2];
	t_frmt_fs			frmt;

	va_start(arg, format);
	inputer[0] = (char *)format;
	n[1] = 0;
	while ((inputer[1] = ft_strchr(inputer[0], '%')))
	{
		write(STDOUT_FILENO, inputer[0], (n[0] = inputer[1] - inputer[0]));
		n[1] += n[0];
		if (*(inputer[1]) == '%' && ++inputer[1])
		{
			inputer[1] = format_parser(inputer[1], &frmt, arg);
			n[1] += insert_spec(&frmt, arg);
		}
		inputer[0] = inputer[1];
	}
	n[0] = ft_strlen(inputer[0]);
	write(STDOUT_FILENO, inputer[0], n[0]);
	va_end(arg);
	return ((int)(n[1] + n[0]));
}
