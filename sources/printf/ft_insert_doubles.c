/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insert_doubles.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 14:52:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/20 09:45:26 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include "longnumber.h"

int	inline		put_fl(t_frmt_fs *f, const char *fl, \
const char *fint, size_t len)
{
	if (!f->orient && !f->zerofill)
		len += ft_putchar_n((f->zerofill ? '0' : ' '), f->field_len - len);
	if (f->error)
		write(STDOUT_FILENO, "-", 1);
	else if (f->sign == 1)
		write(STDOUT_FILENO, "+", 1);
	else if (f->sign == 2)
		write(STDOUT_FILENO, " ", 1);
	if (!f->orient && f->zerofill)
		len += ft_putchar_n((f->zerofill ? '0' : ' '), f->field_len - len);
	write(STDOUT_FILENO, fint, ft_strlen(fint));
	if (f->precision || (f->sharp && ++len))
		write(STDOUT_FILENO, ".", 1);
	if (f->precision)
		write(STDOUT_FILENO, fl, ft_strlen(fl));
	if (f->orient)
		len += ft_putchar_n(' ', f->field_len - len);
	return (len);
}

char			*check_the_infs(t_frmt_fs *f, const long double a)
{
	const t_8b		*mask = (t_8b *)&a;

	if ((mask[1] & LDOUBLE_EXP_MASK) == LDOUBLE_EXP_MASK)
	{
		f->precision = 0;
		f->zerofill = 0;
		f->sharp = 0;
		if (!(mask[0] << 1) && f->spec <= 'Z')
			return (ft_strdup("INF"));
		if (!(mask[0] << 1) && f->spec >= 'a')
			return (ft_strdup("inf"));
		f->error = 112;
		f->sign = 0;
		if (f->spec <= 'Z')
			return (ft_strdup("NAN"));
		if (f->spec >= 'a')
			return (ft_strdup("nan"));
	}
	return (0);
}

ssize_t			print_spc_f(t_frmt_fs *f, const long double a)
{
	char		*str[2];
	char		over;
	const int	sig = (*((t_8b *)&a + 1) & (t_8b)0x8000) >> 15;
	ssize_t		len;

	over = 0;
	str[1] = (char *)1;
	if (!(str[0] = check_the_infs(f, a)) && f->precision)
		str[1] = ln_string_fl_part(f->precision, ln_from_ldouble(a), &over);
	else if ((((long double)(a - (size_t)a) >= 0.5L && (size_t)a % 2) ||\
	(!((size_t)a % 2) && (long double)(a - (size_t)a) > 0.5L)) && (((*((\
	size_t *)&a + 1) & LDOUBLE_EXP_MASK) - LDOUBLE_EXP_SHIFT) < 64) && !str[0])
		over = 1;
	str[0] = str[0] ? str[0] : ln_string_int_part(a, over);
	f->error = str[0] && str[1] ? f->error : 34;
	str[1] == (char *)1 ? str[1] = 0 : 0;
	len = f->precision ? f->precision + 1 : 0;
	if (f->error == 34)
		return (-1);
	len += ft_strlen(str[0]) + (sig || f->sign > 0 ? 1 : 0);
	f->error = f->error != 112 ? sig : 0;
	len = put_fl(f, str[1], str[0], len);
	free(str[0]);
	str[1] ? free(str[1]) : 0;
	return (len);
}

ssize_t			insert_doubles(t_frmt_fs *frmt, va_list arg)
{
	if (frmt->ispre == 0)
	{
		frmt->ispre = 1;
		frmt->precision = 6;
	}
	if ((frmt->spec == 'f' || frmt->spec == 'F') \
	&& frmt->size != sizeof(long double))
		return (print_spc_f(frmt, va_arg(arg, double)));
	else if ((frmt->spec == 'f' || frmt->spec == 'F'))
		return (print_spc_f(frmt, va_arg(arg, long double)));
	else if ((frmt->spec == 'e' || frmt->spec == 'E') \
	&& frmt->size != sizeof(long double))
		return (print_spc_f(frmt, va_arg(arg, double)));
	else if ((frmt->spec == 'e' || frmt->spec == 'E'))
		return (print_spc_f(frmt, va_arg(arg, long double)));
	else
		return (-1);
}
