/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bigint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 14:56:16 by hgranule          #+#    #+#             */
/*   Updated: 2019/06/30 15:11:38 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "longnumber.h"
#include "libftprintf.h"

t_bi			ln_pow_two(t_8b pow)
{
	t_bi		bi;
	int			i;

	ft_bzero(&bi, sizeof(bi));
	bi.num[0] = 1;
	while (pow--)
	{
		i = LINT_SIZE;
		while (--i > -1)
		{
			bi.num[i] <<= 1;
			if (bi.num[i] >= LINT_OVER)
			{
				bi.num[i] -= LINT_OVER;
				++bi.num[i + 1];
			}
		}
	}
	bi.num[LINT_SIZE] = 0;
	return (bi);
}

t_bi			ln_bint_add(t_bi a, t_bi b)
{
	t_bi		res;
	int			i;

	ft_bzero(&res, sizeof(res));
	i = -1;
	while (++i < LINT_SIZE)
	{
		res.num[i] += a.num[i] + b.num[i];
		if (res.num[i] >= LINT_OVER)
		{
			if (res.num[i] >= LINT_OVER)
				res.num[i] -= LINT_OVER;
			res.num[i + 1] = 1;
		}
	}
	res.num[LINT_SIZE] = 0;
	return (res);
}

t_bi			ln_get_bint(t_8b mnt, int exp)
{
	t_bi		bint;

	ft_bzero(&bint, sizeof(bint));
	while (exp > -1)
	{
		if (0x8000000000000000 & mnt)
			bint = ln_bint_add(bint, ln_pow_two(exp));
		mnt <<= 1;
		--exp;
	}
	return (bint);
}

t_bi			ln_get_t8byte(const long double ld)
{
	t_bi		bint;
	int			exp;
	t_8b const	*lp = (t_8b *)&ld;

	if ((LDOUBLE_EXP_MASK & lp[1]) >= LDOUBLE_EXP_SHIFT)
	{
		exp = (LDOUBLE_EXP_MASK & lp[1]) - LDOUBLE_EXP_SHIFT;
		bint = ln_get_bint(lp[0], exp);
	}
	else
		ft_bzero(&bint, sizeof(bint));
	return (bint);
}

char			*ln_string_int_part(long double ld, char over)
{
	t_bi			bint;
	const t_bi		bi1 = ln_get_bint(0x8000000000000000, 0);
	int				i;
	char			*str[2];
	size_t			len;

	i = LINT_SIZE;
	str[0] = ft_strnew(5000);
	bint = ln_get_t8byte(ld);
	if (over)
		bint = ln_bint_add(bint, bi1);
	while (bint.num[i] == 0 && i > 0)
		--i;
	str[1] = ft_lltoa_base(bint.num[i--], 10);
	len = ft_strlen(str[1]);
	ft_memcpy(str[0], str[1], len);
	free(str[1]);
	str[1] = str[0] + len;
	while (i > -1)
		str[1] = ln_n2s_rformat(bint.num[i--], 18, '0', str[1]);
	return (str[0]);
}
