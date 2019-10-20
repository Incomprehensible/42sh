/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bf_shifts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 15:04:20 by hgranule          #+#    #+#             */
/*   Updated: 2019/06/30 15:04:30 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "longnumber.h"

t_lnum			ln_lowshift_mnt(t_lnum num, t_4b shift)
{
	t_8b		sup[LNUM_BYTES8 * 2];
	t_4b		bytes;
	t_4b		bits;
	int			i;

	shift %= LNUM_BYTES8 * 8 * 8;
	ft_bzero(sup, sizeof(sup));
	bytes = shift / 64;
	bits = shift % 64;
	ft_memcpy(&(sup[LNUM_BYTES8 - bytes]), num.mnt, LNUM_BYTES8 * 8);
	ft_memcpy(num.mnt, &(sup[LNUM_BYTES8]), LNUM_BYTES8 * 8);
	i = 0;
	while (i < LNUM_BYTES8)
	{
		sup[i + LNUM_BYTES8 - 1] = num.mnt[i] << (64 - bits);
		num.mnt[i] >>= bits;
		i++;
	}
	sup[LNUM_BYTES8 * 2 - 1] = 0;
	while (--i && shift)
		num.mnt[i] |= sup[i + LNUM_BYTES8];
	return (num);
}

t_lnum			ln_highshift_mnt(t_lnum num, t_4b shift)
{
	t_8b		sup[LNUM_BYTES8 * 2];
	t_4b		bytes;
	t_4b		bits;
	int			i;

	shift %= LNUM_BYTES8 * 8 * 8;
	ft_bzero(sup, sizeof(sup));
	bytes = shift / 64;
	bits = shift % 64;
	ft_memcpy(&(sup[bytes]), num.mnt, LNUM_BYTES8 * 8);
	ft_memcpy(num.mnt, &(sup[0]), LNUM_BYTES8 * 8);
	i = 0;
	while (i < LNUM_BYTES8)
	{
		sup[i + LNUM_BYTES8 + 1] = num.mnt[i] >> (64 - bits);
		num.mnt[i] <<= bits;
		i++;
	}
	sup[LNUM_BYTES8] = 0;
	while (--i && shift)
		num.mnt[i] |= sup[i + LNUM_BYTES8];
	return (num);
}

t_lnum			ln_lowshift(t_lnum num, t_4b shift)
{
	num.exponent += shift;
	num = ln_lowshift_mnt(num, shift);
	return (num);
}

t_lnum			ln_highshift(t_lnum num, t_4b shift)
{
	num.exponent -= shift;
	num = ln_highshift_mnt(num, shift);
	return (num);
}
