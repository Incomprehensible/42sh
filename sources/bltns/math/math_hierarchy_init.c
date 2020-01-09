/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_hierarchy_init.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 00:05:20 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

void	first_level_in(t_tk_type *arr)
{
	arr[0] = INCRM;
	arr[1] = DECRM;
	arr[2] = NEGAT;
	arr[3] = REJECT;
	arr[4] = NOT;
	arr[5] = POSIT;
	arr[6] = 0;
}

void	second_level_in(t_tk_type *arr)
{
	arr[0] = BIT_L;
	arr[1] = BIT_R;
	arr[2] = 0;
}

void	third_level_in(t_tk_type *arr)
{
	arr[0] = AND;
	arr[1] = OR;
	arr[2] = XOR;
	arr[3] = 0;
}

long	normal_ops(long a, long b, t_tk_type op, ERR *err)
{
	long res;

	if (op == DEVID || op == DEVREM)
	{
		if (b == 0)
		{
			set_error(ft_itoa(b), DIVISION_ZERO, err);
			return (0);
		}
	}
	res = op == PLUS ? a + b : 0;
	res = op == MINUS ? a - b : res;
	res = op == MULT ? a * b : res;
	res = op == DEVID ? a / b : res;
	res = op == DEVREM ? a % b : res;
	return (res);
}

short	get_op_type(t_tk_type op)
{
	if (op == INCRM || op == DECRM || op == POSIT || op == NEGAT ||
	op == REJECT || op == NOT)
		return (1);
	else if (op == EQU)
		return (3);
	return (2);
}
