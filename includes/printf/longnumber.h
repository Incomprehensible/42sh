/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   longnumber.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 11:52:11 by hgranule          #+#    #+#             */
/*   Updated: 2019/07/04 19:26:51 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LONGNUMBER_H
# define LONGNUMBER_H

# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

typedef unsigned long	t_8b;
typedef unsigned int	t_4b;
typedef unsigned char	t_1b;

# define LNUM_EXP_4SHIFT 0x7FFFFFFF

# define LDOUBLE_EXP_MASK 0x7FFF
# define LDOUBLE_EXP_SHIFT 0x3FFF

# define LNUM_BYTES8 64

# define LINT_SIZE 278
# define LINT_OVER 1000000000000000000U

typedef struct	s_longnumber
{
	t_8b		mnt[LNUM_BYTES8];
	t_4b		exponent;
	t_1b		sign;
}				t_lnum;

typedef struct	s_bigint
{
	t_8b		num[LINT_SIZE + 2];
	t_1b		err;
}				t_bi;

long double		ln_to_ldouble(t_lnum lnum);
t_lnum			ln_multint_mnt(t_lnum n1, t_8b ni);
t_lnum			ln_sub(t_lnum n1, t_lnum n2);
t_lnum			ln_sub_mnt(t_lnum n1, t_lnum n2);
t_lnum			ln_summ_mnt(t_lnum n1, t_lnum n2);
int				ln_cmp(t_lnum n1, t_lnum n2);
int				ln_abscmp(t_lnum n1, t_lnum n2);
t_lnum			ln_lowshift(t_lnum num, t_4b shift);
t_lnum			ln_lowshift_mnt(t_lnum num, t_4b shift);
t_lnum			ln_highshift(t_lnum num, t_4b shift);
t_lnum			ln_highshift_mnt(t_lnum num, t_4b shift);
t_lnum			ln_from_ldouble(long double ldob);
t_8b			ln_to_ulong(t_lnum n);
char			*ln_string_fl_part(int prec, t_lnum lnum, char *over);

char			*ln_string_int_part(long double ld, char over);
t_bi			ln_get_t8byte(const long double ld);
t_bi			ln_get_bint(t_8b mnt, int exp);
t_bi			ln_bint_add(t_bi a, t_bi b);
t_bi			ln_pow_two(t_8b pow);
char			*ln_n2s_rformat(t_8b a, const int ln, const char fil, char *s);

#endif
