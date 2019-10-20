/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 20:11:47 by hgranule          #+#    #+#             */
/*   Updated: 2019/07/04 17:23:45 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static char	*flag_parser(char *sc, t_frmt_fs *frmt)
{
	while (1)
	{
		if (*sc == '0')
			!frmt->orient ? frmt->zerofill = 1 : 0;
		else if (*sc == '+')
			frmt->sign = 1;
		else if (*sc == '-')
			frmt->orient = 1;
		else if (*sc == ' ')
			frmt->sign != 1 ? frmt->sign = 2 : 0;
		else if (*sc == '#')
			frmt->sharp = 1;
		else
			break ;
		++sc;
	}
	return (sc);
}

static char	*wp_parser(char *sc, t_frmt_fs *frmt, va_list arg)
{
	if (*sc == '*' && sc++)
		frmt->field_len = va_arg(arg, int);
	else if ((frmt->field_len = ft_abc(ft_atoi(sc))))
		while (ft_isdigit(*sc))
			sc++;
	if (*sc == '.')
	{
		frmt->ispre = 1;
		if (*(++sc) == '*' && sc++)
			frmt->precision = va_arg(arg, int);
		else if ((frmt->precision = ft_abc(ft_atoi(sc))))
			while (ft_isdigit(*sc))
				sc++;
	}
	if (frmt->field_len < 0 && (frmt->field_len *= -1))
		frmt->orient = 1;
	if (frmt->precision < 0 && !(frmt->precision = 0))
		frmt->ispre = 0;
	return (sc);
}

static int	type_mode(char *sc)
{
	if (ft_memchr("dDioOxXuUb", (int)(*sc), 10))
		return (1);
	else if (ft_memchr("aAeEfFgG", (int)(*sc), 8))
		return (2);
	else if (*sc == 's')
		return (3);
	else if (*sc == 'c')
		return (4);
	else if (*sc == 'p')
		return (5);
	else if (*sc == 'r' || *sc == 'R')
		return (27);
	else if (*sc == '%')
		return (6);
	else if (ft_memchr("yqwt[{]}jkm<>?", *sc, 15))
		return (-1);
	return (0);
}

static char	*type_parser(char *sc, t_frmt_fs *f)
{
	long double	m;

	f->size = 4;
	while (!(f->type = type_mode(sc)) && *sc && f->type != -1)
	{
		!(ft_strncmp("hh", sc, 2)) && f->size < (int)sizeof(long)\
		? f->size = sizeof(char) : 0;
		*sc == 'h' && f->size < (int)8 && f->size > (int)sizeof(char)\
		? f->size = sizeof(short) : 0;
		*sc == 'l' && f->size < (int)sizeof(m) ? f->size = sizeof(long) : 0;
		!(ft_strncmp("ll", sc, 2)) && f->size < (int)sizeof(m) \
		? f->size = sizeof(long long) : 0;
		*sc == 'z' && f->size < (int)sizeof(m) ? f->size = sizeof(size_t) : 0;
		*sc == 't' && f->size < (int)sizeof(m) ? f->size = sizeof(size_t) : 0;
		*sc == 'j' && f->size < (int)sizeof(m) ? f->size = sizeof(size_t) : 0;
		*sc == 'L' ? f->size = sizeof(m) : 0;
		++sc;
	}
	f->type == 1 && f->size > 8 ? f->size = 8 : 0;
	f->type == 2 && !f->size ? f->size = 8 : 0;
	f->type == 2 && f->size < 8 ? f->size = 8 : 0;
	f->type == 3 || f->type == 5 ? f->size = 8 : 0;
	f->type == 4 ? f->size = 1 : 0;
	*sc && f->type != -1 ? ++sc : 0;
	return (sc);
}

char		*format_parser(char *sc, t_frmt_fs *frmt, va_list arg)
{
	ft_bzero(frmt, sizeof(t_frmt_fs));
	sc = flag_parser(sc, frmt);
	sc = wp_parser(sc, frmt, arg);
	sc = type_parser(sc, frmt);
	frmt->spec = *(sc - 1);
	return (sc);
}
