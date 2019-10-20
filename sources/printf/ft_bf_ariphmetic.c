/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bf_ariphmetic.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 15:05:12 by hgranule          #+#    #+#             */
/*   Updated: 2019/07/04 00:21:52 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "longnumber.h"

t_lnum			ln_summ_mnt(t_lnum n1, t_lnum n2)
{
	int			i;
	t_lnum		sum;

	ft_bzero(&sum, sizeof(sum));
	if (n1.exponent > n2.exponent)
		n2 = ln_lowshift(n2, n1.exponent - n2.exponent);
	else if (n2.exponent > n1.exponent)
		n1 = ln_lowshift(n1, n2.exponent - n1.exponent);
	i = -1;
	sum.exponent = n1.exponent;
	while (++i < LNUM_BYTES8 - 1)
	{
		sum.mnt[i] += n1.mnt[i] + n2.mnt[i];
		if ((sum.mnt[i] < n1.mnt[i]) || (sum.mnt[i] < n2.mnt[i]))
			++sum.mnt[i + 1];
	}
	sum.mnt[i] += n1.mnt[i] + n2.mnt[i];
	if ((sum.mnt[i] < n1.mnt[i]) || (sum.mnt[i] < n2.mnt[i]))
	{
		sum = ln_lowshift(sum, 1);
		sum.mnt[i] += 0x8000000000000000;
	}
	return (sum);
}

t_lnum			ln_sub_mnt(t_lnum n1, t_lnum n2)
{
	int			i;
	t_lnum		sub;

	if (((i = ln_cmp(n1, n2)) < 0))
		n1 = ln_lowshift(n1, n2.exponent - n1.exponent);
	else if (i > 0)
		n2 = ln_lowshift(n2, n1.exponent - n2.exponent);
	else
		return (ln_from_ldouble(0.0));
	ft_bzero(&sub, sizeof(sub));
	sub.exponent = n1.exponent;
	if (i < 0)
		sub.exponent = n2.exponent;
	i = 0;
	while (i < (LNUM_BYTES8 - 1))
	{
		sub.mnt[i] += n1.mnt[i] - n2.mnt[i];
		if (sub.mnt[i] > n1.mnt[i])
			--sub.mnt[i + i];
		++i;
	}
	sub.mnt[i] += n1.mnt[i] - n2.mnt[i];
	while (!(sub.mnt[LNUM_BYTES8 - 1] & 0x8000000000000000) && (i-- > -1))
		sub = ln_highshift(sub, 1);
	return (sub);
}

t_lnum			ln_sub(t_lnum n1, t_lnum n2)
{
	t_lnum		sub;
	int			cp;

	ft_bzero(&sub, sizeof(sub));
	if (!n1.exponent && n2.exponent)
	{
		n2.sign = !n2.sign;
		return (n2);
	}
	else if (!n2.exponent)
		return (n1);
	if (((cp = ln_cmp(n1, n2)) > 0))
		sub = ln_sub_mnt(n1, n2);
	else if (cp < 0)
	{
		sub = ln_sub_mnt(n2, n1);
		sub.sign = !sub.sign;
	}
	return (sub);
}

t_lnum			ln_multint_mnt(t_lnum n1, t_8b ni)
{
	t_lnum	sum;

	ft_bzero(&sum, sizeof(sum));
	while (ni--)
		sum = ln_summ_mnt(sum, n1);
	return (sum);
}
