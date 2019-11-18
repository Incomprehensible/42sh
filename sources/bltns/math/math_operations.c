/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/07 05:01:32 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

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

long	bit_ops(long a, long b, t_tk_type op, ERR *err)
{
	long res;

	res = op == BIT_R ? a >> b : 0;
	res = op == BIT_L ? a << b : res;
	res = op == AND ? a & b : res;
	res = op == OR ? a | b : res;
	res = op == XOR ? a ^ b : res;
	return (res);
}

long	compare_ops(long a, long b, t_tk_type op, ERR *err)
{
	long res;

	res = op == IS_EQU ? a == b : 0;
	res = op == NO_EQU ? a != b : res;
	res = op == MORE ? a > b : res;
	res = op == LESS ? a < b : res;
	res = op == MORE_EQ ? a >= b : res;
	res = op == LESS_EQ ? a <= b : res;
	return (res);
}

long	logic_ops(long a, long b, t_tk_type op, ERR *err)
{
	long res;

	res = op == LOG_AND ? a && b : 0;
	res = op == LOG_OR ? a || b : res;
	return (res);
}

long	assign_ops(long a, long b, t_tk_type op, ERR *err)
{
	long res;

	res = op == PLUS_EQ ? a += b : 0;
	res = op == MIN_EQ ? a -= b : res;
	return (res);
}

long	apply_to_single(long var, t_tk_type op)
{
	long res;

	res = op == INCRM ? ++var : 0;
	res = op == DECRM ? --var : res;
	res = op == POSIT ? var * 1 : res;
	res = op == NEGAT ? var * -1 : res;
	res = op == REJECT ? !(var) : res;
	res = op == NOT ? ~var : res;
	return (res);
}