/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 03:02:48 by hgranule          #+#    #+#             */
/*   Updated: 2019/07/04 17:00:55 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdarg.h>
# include "libft.h"
# include "longnumber.h"

/*
** error - зарезервиравано под ошибки
** type - типы спецификаторов от 1 до 5
** size - размер в байтах
** field_len - длина поля (числа по пизде)
** precision - точность
** orient - если 0 то правое, если 1 то левое выравнивания
** zerofill - заполнение нулями
** sharp - ШАРП
** sign - знак
** ispre - есть ли пресижн
** spec - спецификатор
*/
typedef struct		s_flags_specifier
{
	int				error;
	int				type;
	int				size;
	int				field_len;
	int				precision;
	char			orient;
	char			zerofill;
	char			asteriks;
	char			sharp;
	char			sign;
	char			ispre;
	char			spec;
}					t_frmt_fs;

int					ft_printf(const char *format, ...);

/*
** FORMAT PARSER FUCNTIONS
*/

char				*format_parser(char *spec, t_frmt_fs *frmt, va_list arg);

/*
** INSERTING FUNCTIONS
*/

size_t				insert_spec(t_frmt_fs *frmt, va_list arg);
int					ft_putchar_n(char c, ssize_t n);

/*
** INSERT STRING
*/

size_t				ft_insert_s(t_frmt_fs *frmt, va_list arg);

/*
** INSERT CHAR
*/

int					ft_insert_c(t_frmt_fs *frmt, va_list arg);

/*
** INSERT B
*/

int					ft_insert_b(t_frmt_fs *frmt, va_list arg);

/*
** HELP
*/

void				ft_putll_base_spec(__int128_t nb, char spec);
int					ft_return_insert_b(t_frmt_fs *f, int len);
int					ft_get_len(long long *n, int base, t_frmt_fs *f);
int					ft_parse_spec(t_frmt_fs *f, int fl, long long n);
int					ft_get_base(char c);
int					ft_kostyl_zero(t_frmt_fs *f, int len);
int					ft_kostyl_sign(t_frmt_fs *f);
int					ft_abc(int n);

/*
** INSERT P
*/

int					ft_insert_p(t_frmt_fs *f, va_list arg);

/*
** FLOAT INSERTING
*/

int					put_fl(t_frmt_fs *f, const char *fl, \
const char *i, size_t l);
ssize_t				insert_doubles(t_frmt_fs *frtmt, va_list arg);

/*
** NON_PRINTABLE_CHARACTER
*/

ssize_t				ft_insert_r(t_frmt_fs *f, va_list arg);

/*
** INSERT percents
*/

size_t				ft_insert_percent(t_frmt_fs *f);

#endif
