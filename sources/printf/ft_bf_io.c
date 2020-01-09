/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bf_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 14:59:23 by hgranule          #+#    #+#             */
/*   Updated: 2019/07/04 12:30:52 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "longnumber.h"
#include "libft.h"

t_lnum			ln_from_ldouble(long double ldob)
{
	t_lnum		lnm;
	t_8b		*bytedouble8;

	bytedouble8 = (t_8b *)&ldob;
	ft_bzero(&lnm, sizeof(lnm));
	lnm.sign = (bytedouble8[1] & 0x8000) >> 15;
	lnm.exponent = ((bytedouble8[1] & LDOUBLE_EXP_MASK)\
	- LDOUBLE_EXP_SHIFT) + LNUM_EXP_4SHIFT;
	lnm.mnt[LNUM_BYTES8 - 1] = bytedouble8[0];
	return (lnm);
}

t_8b			ln_to_ulong(const t_lnum n)
{
	t_8b			ulint;
	t_4b			bin_ds;

	bin_ds = 64 - (n.exponent - LNUM_EXP_4SHIFT + 1);
	ulint = n.mnt[LNUM_BYTES8 - 1];
	if (n.exponent > 63 + (t_4b)LNUM_EXP_4SHIFT)
		return ((t_8b)-1);
	if (n.exponent < LNUM_EXP_4SHIFT)
		return ((t_8b)0);
	ulint >>= bin_ds;
	return (ulint);
}

long double		ln_to_ldouble(const t_lnum lnum)
{
	long double		ldob;
	t_8b			*bytedouble8;

	bytedouble8 = (t_8b *)&ldob;
	bytedouble8[1] = 0x8000 & (lnum.sign << 15);
	bytedouble8[1] |= \
	((lnum.exponent - LNUM_EXP_4SHIFT + LDOUBLE_EXP_SHIFT) & LDOUBLE_EXP_MASK);
	bytedouble8[0] = lnum.mnt[LNUM_BYTES8 - 1];
	return (ldob);
}

char			*ln_string_fl_part(int prec, t_lnum lnum, char *over)
{
	t_8b		i;
	t_8b		j;
	char		*str;

	i = 0;
	str = ft_strnew(prec + 2);
	while (prec-- > -1 && str)
	{
		lnum = ln_multint_mnt(ln_sub_mnt(lnum, \
		ln_from_ldouble((long double)ln_to_ulong(lnum))), 10);
		str[i++] = ln_to_ulong(lnum) + '0';
		str[i - 1] = str[i - 1] == '/' ? '0' : str[i - 1];
	}
	if (str && str[(j = --i)] >= '5')
	{
		while (i > 0 && str[--i] == '9')
			str[i] = '0';
		if (i > 0)
			++str[i];
		else
			*over = 1;
	}
	str ? str[j] = 0 : 0;
	return (str);
}
