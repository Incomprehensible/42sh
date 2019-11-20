/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putll_base_spec.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 15:35:23 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/20 07:14:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

long long			get_max_dec(long long nb, int base)
{
	long long			dec;
	__int128_t			a;

	dec = 1;
	a = nb;
	a < 0 ? a *= -1 : a;
	while ((dec * base < a) && (dec * base > 0))
		dec *= base;
	return (dec);
}

unsigned long long	get_max_dec_u(unsigned long long nb, int base)
{
	size_t			dec;

	dec = 1;
	while ((dec * base < nb) && dec < dec * base && (dec * base > 0))
		dec *= base;
	return (dec);
}

int					ft_get_base(char c)
{
	if (c == 'b')
		return (2);
	if (c == 'o' || c == 'O')
		return (8);
	if (c == 'x' || c == 'X')
		return (16);
	return (10);
}

void				ft_putull_base_spec(unsigned long long nb, char spec)
{
	unsigned long long		dec;
	int						cur;
	char					*alphu;
	char					*alph;
	int						base;

	alphu = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
	alph = "0123456789abcdefghijklmnopqrstuv";
	base = ft_get_base(spec);
	dec = get_max_dec_u(nb, base);
	while (dec > 0)
	{
		if ((cur = nb / dec) == base)
			write(1, "10", 2);
		else
			write(1, spec == 'X' ? &(alphu[cur]) : &(alph[cur]), 1);
		nb %= dec;
		dec /= base;
	}
}

void				ft_putll_base_spec(__int128_t nb, char spec)
{
	__int128_t		dec;
	int				cur;
	char			*alphu;
	char			*alph;
	int				base;

	alphu = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
	alph = "0123456789abcdefghijklmnopqrstuv";
	base = ft_get_base(spec);
	dec = get_max_dec(nb, base);
	if (!ft_memchr("uUoOxX", spec, 6))
	{
		(nb < 0) ? nb *= -1 : 0;
		while (dec > 0)
		{
			if ((cur = nb / dec) == base)
				write(1, "10", 2);
			else
				write(1, spec == 'X' ? &(alphu[cur]) : &(alph[cur]), 1);
			nb %= dec;
			dec /= base;
		}
	}
	else
		ft_putull_base_spec(nb, spec);
}
