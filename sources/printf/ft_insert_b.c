/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insert_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/23 13:10:50 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/20 09:45:26 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

int					ft_parse_spec(t_frmt_fs *f, int fl, long long n)
{
	if (fl == 1 && (f->spec == 'o' || f->spec == 'O') && f->sharp && n != 0)
		return (write(STDOUT_FILENO, "0", 1));
	else if (fl == 1 && f->sharp
	&& n != 0 && f->spec == 'x')
		return (write(STDOUT_FILENO, "0x", 2));
	else if (fl == 1 && f->sharp
	&& n != 0 && f->spec == 'X')
		return (write(STDOUT_FILENO, "0X", 2));
	else if (fl == 1 && f->sharp
	&& n != 0 && f->spec == 'b')
		return (write(STDOUT_FILENO, "b", 1));
	else if (f->spec == 'b' && n != 0 && f->sharp)
		return (1);
	else if ((f->spec == 'o' || f->spec == 'O') && n != 0 && f->sharp)
		return (1);
	else if ((f->spec == 'X' || f->spec == 'x') && n != 0 && f->sharp)
		return (2);
	return (0);
}

long long			ft_get_num(t_frmt_fs *f, va_list arg)
{
	if (f->size == 1 && ft_memchr("uoxXb", (int)f->spec, 5))
		return ((unsigned char)va_arg(arg, int));
	if (f->size == 1)
		return ((char)va_arg(arg, int));
	else if (f->size == 2 && ft_memchr("uoxXb", (int)f->spec, 5))
		return ((unsigned short)va_arg(arg, int));
	else if (f->size == 2)
		return ((short)va_arg(arg, int));
	else if (f->size == 4 && ft_memchr("uoxXb", (int)f->spec, 5))
		return ((unsigned)va_arg(arg, int));
	else if (f->size == 4 && ft_memchr("di", (int)f->spec, 2))
		return (va_arg(arg, int));
	else if (f->size == 8 || ft_memchr("uUoOxXb", (int)f->spec, 7))
		return (va_arg(arg, unsigned long long));
	else if (f->size == 8 || f->spec == 'D')
		return (va_arg(arg, long long));
	return (0);
}

static void			ft_print_b_else(t_frmt_fs *f, long long n, char *s, int l)
{
	int		specl;

	if ((!ft_strcmp(s, "-") && !ft_memchr("uUoOxXb", (int)f->spec, 7))
	|| f->sign == 1)
		write(STDOUT_FILENO, s, 1);
	specl = ft_parse_spec(f, 1, n);
	if (f->spec == 'o' || f->spec == 'O')
		ft_putchar_n('0', f->precision - (l));
	else
		ft_putchar_n('0', f->precision - (l - specl));
	ft_putll_base_spec(n, f->spec);
	if (f->spec == 'o' || f->spec == 'O')
		ft_putchar_n(' ', f->field_len -\
		(f->precision > l - specl ? f->precision : l));
	else
		ft_putchar_n(' ', f->field_len -\
		(f->precision > l - specl ? f->precision + specl : l));
}

static void			ft_print_b(t_frmt_fs *f, long long n, char *sign, int len)
{
	int		fl;
	int		spc[2];

	if ((fl = 0) == 0 && f->orient)
		return (ft_print_b_else(f, n, sign, len));
	if (f->field_len > f->precision && f->field_len > len)
	{
		if (((!f->ispre) && f->zerofill
		&& (*sign == '-' || f->sign == 1) && (fl = 1))
		&& !ft_memchr("uUoOxXb", (int)f->spec, 7))
			write(STDOUT_FILENO, sign, 1);
		spc[0] = ft_parse_spec(f, f->zerofill && !f->ispre, n);
		spc[1] = (f->spec == 'o' || f->spec == 'O') ? 0 : spc[0];
		ft_putchar_n(f->zerofill && !f->ispre ? '0' : ' ', f->field_len -\
		(f->precision + spc[0] > len ? f->precision + spc[1] : len));
	}
	if (!fl && !ft_memchr("uUoOxXb", (int)f->spec, 7)
	&& (*sign == '-' || f->sign == 1))
		write(STDOUT_FILENO, sign, 1);
	ft_parse_spec(f, !f->zerofill || (f->field_len <= len) || f->ispre, n);
	ft_putchar_n('0', f->sharp && (f->spec == 'X' || f->spec == 'x')
	&& n != 0 ? f->precision - len + 2 : f->precision - len);
	ft_putll_base_spec(n, f->spec);
}

int					ft_insert_b(t_frmt_fs *f, va_list arg)
{
	long long	n;
	char		*sign;
	int			len;
	int			spec_len;

	n = ft_get_num(f, arg);
	sign = n >= 0 ? "+" : "-";
	len = ft_get_len(&n, ft_get_base(f->spec), f);
	len += (int)(f->sign && *sign == '+' && !ft_memchr("uUoOxXb", f->spec, 7));
	f->precision += f->precision && (f->sign || !ft_strcmp(sign, "-")) ? 1 : 0;
	if (f->ispre && !f->precision && !n && !f->sign)
		return (ft_kostyl_zero(f, len));
	if (n == 0 && f->ispre && !f->precision && f->sign)
		return (ft_kostyl_sign(f));
	if (f->sign == 2 && *sign == '+' && !ft_memchr("uUoOxXb", f->spec, 7))
		write(STDOUT_FILENO, " ", 1);
	ft_print_b(f, n, sign, len);
	spec_len = ft_parse_spec(f, 0, n);
	(spec_len == 1) ? --f->precision : 0;
	len - spec_len < f->precision ? len = f->precision + spec_len : 0;
	return (ft_return_insert_b(f, len));
}
