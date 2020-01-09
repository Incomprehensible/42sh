/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bf_cmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 15:03:18 by hgranule          #+#    #+#             */
/*   Updated: 2019/06/30 15:03:32 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "longnumber.h"

int				ln_abscmp(t_lnum n1, t_lnum n2)
{
	int		i;

	if (n1.exponent > n2.exponent)
		return (1);
	else if (n2.exponent > n1.exponent)
		return (-1);
	i = LNUM_BYTES8;
	while (i--)
	{
		if (n1.mnt[i] > n2.mnt[i])
			return (1);
		if (n2.mnt[i] > n1.mnt[i])
			return (-1);
	}
	return (0);
}

int				ln_cmp(t_lnum n1, t_lnum n2)
{
	(!n1.exponent ? n1.exponent = LNUM_EXP_4SHIFT : 0);
	(!n2.exponent ? n2.exponent = LNUM_EXP_4SHIFT : 0);
	if (!n1.exponent && !n2.exponent)
		return (0);
	if (n1.sign == n2.sign)
	{
		if (!n1.sign)
			return (ln_abscmp(n1, n2));
		else
			return (-1 * ln_abscmp(n1, n2));
	}
	else
	{
		if (n1.sign > n2.sign)
			return (-1);
		else
			return (1);
	}
}
