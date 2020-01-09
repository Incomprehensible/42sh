/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insert_spec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 20:53:55 by hgranule          #+#    #+#             */
/*   Updated: 2019/07/04 04:29:56 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

size_t		insert_spec(t_frmt_fs *frmt, va_list arg)
{
	if (frmt->type == 1)
		return (ft_insert_b(frmt, arg));
	else if (frmt->type == 2)
		return (insert_doubles(frmt, arg));
	else if (frmt->type == 3)
		return (ft_insert_s(frmt, arg));
	else if (frmt->type == 4)
		return (ft_insert_c(frmt, arg));
	else if (frmt->type == 5)
		return (ft_insert_p(frmt, arg));
	else if (frmt->type == 27)
		return (ft_insert_r(frmt, arg));
	else if (frmt->type == 6)
		return (ft_insert_percent(frmt));
	else
		return (0);
}
