/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_calculate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 01:45:29 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

t_tk_type	get_ind(t_tk_type op)
{
	static t_tk_type	ops[5][8];
	t_tk_type			type;
	t_tk_type			i;

	if (!ops[0][0])
	{
		bit_level_in((t_tk_type *)ops);
		normal_level_in((t_tk_type *)ops);
		compare_level_in((t_tk_type *)ops);
		logic_level_in((t_tk_type *)ops);
		assign_level_in((t_tk_type *)ops);
	}
	type = 0;
	while (type <= 4)
	{
		i = 0;
		while (ops[type][i] && ops[type][i] != op)
			i++;
		if (ops[type][i] && ops[type][i] == op)
			return (type);
		type++;
	}
	return (type);
}

t_dlist		*get_both_opd(t_dlist *opd_stack, t_tk_type op, ENV *env, ERR *err)
{
	static long	(*ptr[5])(long, long, t_tk_type, ERR *);
	long		res;
	long		a;
	long		b;

	if (!ptr[0])
	{
		ptr[0] = bit_ops;
		ptr[1] = normal_ops;
		ptr[2] = compare_ops;
		ptr[3] = logic_ops;
		ptr[4] = assign_ops;
	}
	if (!opd_stack || !opd_stack->prev)
		return (process_opd_err(opd_stack, err));
	a = fetch_operand((t_tok *)opd_stack->prev->content, env, err);
	b = fetch_operand((t_tok *)opd_stack->content, env, err);
	if (err->err_code)
		return (opd_stack);
	res = ptr[get_ind(op)](a, b, op, err);
	if (err->err_code)
		return (opd_stack);
	return (substitute_both(opd_stack, env, res, op));
}

t_dlist		*get_single_opd(t_dlist *opd, t_tk_type op, ENV *env, ERR *err)
{
	long	res;
	long	a;

	if (!opd)
		return (set_error(NULL, OPERAND_EXP, err));
	a = fetch_operand((t_tok *)opd->content, env, err);
	if (err->err_code)
		return (opd);
	res = apply_to_single(a, op);
	return (substitute_single(opd, env, res, op));
}

t_dlist		*calculate_res(t_dlist *opd_stack, t_tk_type op, ENV *env, ERR *err)
{
	short	id;

	id = get_op_type(op);
	opd_stack = lst_to_end(opd_stack);
	if (id == 1)
		opd_stack = get_single_opd(opd_stack, op, env, err);
	else if (id == 2)
		opd_stack = get_both_opd(opd_stack, op, env, err);
	else
		opd_stack = equate_opd(opd_stack, env, err);
	return (opd_stack);
}

long		polish_calculate(t_dlist **polish_not, ENV *env, ERR *err)
{
	t_dlist		*start;
	t_dlist		*opd_stack;
	t_tk_type	type;

	start = polish_not[0];
	opd_stack = NULL;
	type = ((t_tok *)start->content)->type;
	while (!err->err_code && start && type != TK_EOF)
	{
		if (is_operand_tok(type))
			opd_stack = push_to_stack(opd_stack, start);
		else
			opd_stack = calculate_res(opd_stack, type, env, err);
		start = start->next;
		type = start ? ((t_tok *)start->content)->type : 0;
	}
	clear_tokens(polish_not, 0);
	return (check_result(opd_stack, env, err));
}
