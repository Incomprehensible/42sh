/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_calculate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/20 22:51:13 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"
#include <stdio.h>
#include <stdlib.h>

static short	is_operand(t_tk_type type)
{
	if (type == OPRND || type == DEC || type == HEX ||
	type == BIN || type == SEV)
		return (1);
	return (0);
}

size_t	count_dlist(t_dlist *list)
{
	size_t	len;

	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	return (len);
}

static long	pull_from_base(char *value, t_tk_type type)
{
	long res;

	if (type == HEX || type == BIN)
		value += 2;
	else if (type == SEV)
		value += (*(value + 1) == 'o') ? 2 : 1;
	res = ft_atoi_base(value, type);
	return (res);
}

t_tk_type	is_number(char *value)
{
	t_tk_type type;

	if (value && (*value == '-' || *value == '+'))
		value++;
	if (parse_base("0x@01@", value))
		type = BIN;
	else if (parse_base("0x@0123456789abcdefABCDEF@", value))
		type = HEX;
	else if (parse_base("0@01234567@", value))
		type = SEV;
	else if (parse_base("0o@01234567@", value))
		type = SEV;
	else if (!(*value == '0' && *(value + 1)) &&
	parse_base("@0123456789@", value))
		type = DEC;
	else
		return (0);
	return (type);
}

long	fetch_operand(t_tok *operand, ENV *env, ERR *err)
{
	DSTRING		*value;
	t_tk_type	type;
	long		res;

	if (operand->type == OPRND)
	{
		value = env_get_variable(operand->value, env);
		if (!value || !(*(value)->txt))
		{
			if (!value)
				return ((long)set_error(NULL, WEIRD_ERR, err));
			free(value->txt);
			value->txt = ft_strdup("0");
			type = 10;
		}
		else if (!(type = is_number(value->txt)))
		{
			set_error(ft_strdup(value->txt), STR_OPERAND, err);
			dstr_del(&value);
			return (0);
		}
		res = pull_from_base(value->txt, type);
		dstr_del(&value);
		return (res);
	}
	return (ft_atoi_base(operand->value, (int)operand->type));
}

long	check_result(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char	*value;
	long	res;

	res = 0;
	if (!err->err_code)
	{
		value = opd_stack ? ((t_tok *)opd_stack->content)->value : NULL;
		if (!value || (count_dlist(opd_stack)) != 1)
		{
			set_error(ft_strdup(value), OPERAND_EXP, err);
			return (0);
		}
		if (((t_tok *)opd_stack->content)->type == OPRND)
			res = fetch_operand((t_tok *)opd_stack->content, env, err);
		else
			res = ft_atoi_base(value, ((t_tok *)opd_stack->content)->type);
	}
	del_tokens(opd_stack);
	return (res);
}

static short	get_op_type(t_tk_type op)
{
	if (op == INCRM || op == DECRM || op == POSIT || op == NEGAT ||
	op == REJECT || op == NOT)
		return (1);
	else if (op == EQU)
		return (3);
	return (2);
}

void	compare_level_in(t_tk_type *arr)
{
	arr[8 * 2 + 0] = IS_EQU;
	arr[8 * 2 + 1] = NO_EQU;
	arr[8 * 2 + 2] = LESS_EQ;
	arr[8 * 2 + 3] = MORE_EQ;
	arr[8 * 2 + 4] = LESS;
	arr[8 * 2 + 5] = MORE;
	arr[8 * 2 + 6] = 0;
}

void	normal_level_in(t_tk_type *arr)
{
	arr[8 * 1 + 0] = PLUS;
	arr[8 * 1 + 1] = MINUS;
	arr[8 * 1 + 2] = MULT;
	arr[8 * 1 + 3] = DEVID;
	arr[8 * 1 + 4] = DEVREM;
	arr[8 * 1 + 5] = 0;
}

void	bit_level_in(t_tk_type *arr)
{
	arr[8 * 0 + 0] = BIT_R;
	arr[8 * 0 + 1] = BIT_L;
	arr[8 * 0 + 2] = AND;
	arr[8 * 0 + 3] = OR;
	arr[8 * 0 + 4] = XOR;
	arr[8 * 0 + 5] = 0;
}

void	logic_level_in(t_tk_type *arr)
{
	arr[8 * 3 + 0] = LOG_AND;
	arr[8 * 3 + 1] = LOG_OR;
	arr[8 * 3 + 2] = 0;
}

void	assign_level_in(t_tk_type *arr)
{
	arr[8 * 4 + 0] = EQU;
	arr[8 * 4 + 1] = PLUS_EQ;
	arr[8 * 4 + 2] = MIN_EQ;
	arr[8 * 4 + 3] = 0;
}

static t_tk_type	get_ind(t_tk_type op)
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

t_dlist	*substitute_single(t_dlist *opd_stack, ENV *env, long res, t_tk_type op)
{
	char	*value;
	DSTRING	*d_value;

	value = ft_itoa(res);
	if (((t_tok *)opd_stack->content)->type == OPRND &&
	(op == INCRM || op == DECRM))
	{
		d_value = dstr_new(value);
		env_set_variable(((t_tok *)opd_stack->content)->value, d_value, env);
		dstr_del(&d_value);
	}
	free(((t_tok *)opd_stack->content)->value);
	((t_tok *)opd_stack->content)->value = value;
	((t_tok *)opd_stack->content)->type = DEC;
	while (opd_stack && opd_stack->prev)
		opd_stack = opd_stack->prev;
	return (opd_stack);
}

t_dlist	*substitute_both(t_dlist *opd_stack, ENV *env, long res, t_tk_type op)
{
	char	*value;
	t_dlist	*tmp;
	DSTRING	*new;

	value = ft_itoa(res);
	tmp = opd_stack->prev;
	del_tokens(opd_stack);
	tmp->next = NULL;
	new = dstr_new(value);
	if (((t_tok *)tmp->content)->type == OPRND &&
	(op == PLUS_EQ || op == MIN_EQ))
		env_set_variable(((t_tok *)tmp->content)->value, new, env);
	dstr_del(&new);
	free(((t_tok *)tmp->content)->value);
	((t_tok *)tmp->content)->value = value;
	((t_tok *)tmp->content)->type = DEC;
	opd_stack = tmp;
	while (opd_stack && opd_stack->prev)
		opd_stack = opd_stack->prev;
	return (opd_stack);
}

t_dlist	*get_single_opd(t_dlist *opd_stack, t_tk_type op, ENV *env, ERR *err)
{
	long	res;
	long	a;

	if (!opd_stack)
		return (set_error(NULL, OPERAND_EXP, err));
	a = fetch_operand((t_tok *)opd_stack->content, env, err);
	if (err->err_code)
		return (opd_stack);
	res = apply_to_single(a, op);
	return (substitute_single(opd_stack, env, res, op));
}

t_dlist	*process_opd_err(t_dlist *opd_stack, ERR *err)
{
	if (opd_stack)
	{
		set_error(ft_strdup(((t_tok *)opd_stack->content)->value), OPERAND_EXP, err);
		return (opd_stack);
	}
	return (set_error(NULL, OPERAND_EXP, err));
}

t_dlist	*get_both_opd(t_dlist *opd_stack, t_tk_type op, ENV *env, ERR *err)
{
	static long			(*ptr[5])(long, long, t_tk_type, ERR *);
	long 				res;
	long				a;
	long				b;

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

char *pull_env_val(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char		*new;
	DSTRING		*value;
	char		*to_str;
	t_tk_type	type;
	long		res;
	
	new = ((t_tok *)opd_stack->content)->value;
	value = env_get_variable(((t_tok *)opd_stack->content)->value, env);
	if (!(*(value->txt)))
	{
	    free(value->txt);
		value->txt = ft_strdup("0");
	}
	if (!(type = is_number(value->txt)))
	{
		set_error(ft_strdup(new), STR_OPERAND, err);
		dstr_del(&value);
		return (NULL);
	}
	res = ft_atoi_base(value->txt, type);
	new = ((t_tok *)opd_stack->prev->content)->value;
	if (((t_tok *)opd_stack->prev->content)->type != OPRND)
	{
		set_error(ft_strdup(new), INVALID_ASSIG, err);
		dstr_del(&value);
		return (NULL);
	}
	dstr_del(&value);
	to_str = ft_itoa(res);
	value = dstr_new(to_str);
	env_set_variable(new, value, env);
	dstr_del(&value);
	return (to_str);
}

char *set_var(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char		*value;
	DSTRING		*fucking_str;
	char		*new;
	t_tk_type	type;
	long		res;
	
	err = 0;
	value = ((t_tok *)opd_stack->content)->value;
	type = ((t_tok *)opd_stack->content)->type;
	res = ft_atoi_base(value, type);
	new = ft_itoa(res);
	value = ((t_tok *)opd_stack->prev->content)->value;
	fucking_str = dstr_new(new);
	env_set_variable(value, fucking_str, env);
	dstr_del(&fucking_str);
	return (new);
}

t_dlist	*set_new_var(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char		*value;

	if (((t_tok *)opd_stack->content)->type == OPRND)
		value = pull_env_val(opd_stack, env, err);
	else
		value = set_var(opd_stack, env, err);
	opd_stack = opd_stack->prev;
	del_tokens(opd_stack->next);
	free(((t_tok *)opd_stack->content)->value);
	((t_tok *)opd_stack->content)->value = value;
	((t_tok *)opd_stack->content)->type = DEC;
	opd_stack->next = NULL;
	while (opd_stack->prev)
		opd_stack = opd_stack->prev;
	return (opd_stack);
}

t_dlist	*equate_opd(t_dlist *opd_stack, ENV *env, ERR *err)
{
	t_tk_type	opd;
	char		*value;

	if (!opd_stack || !opd_stack->prev)
	{
		if (opd_stack)
		{
			set_error(ft_strdup("="), OPERAND_EXP, err);
			return (opd_stack);
		}
		else
			return (set_error(NULL, OPERAND_EXP, err));
	}
	opd = ((t_tok *)opd_stack->prev->content)->type;
	value = ((t_tok *)opd_stack->prev->content)->value;
	if (opd != OPRND)
	{
		set_error(ft_strdup(value), INVALID_ASSIG, err);
		return (opd_stack);
	}
	return (set_new_var(opd_stack, env, err));
}

t_dlist	*calculate_res(t_dlist *opd_stack, t_tk_type op, ENV *env, ERR *err)
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

long	polish_calculate(t_dlist **polish_not, ENV *env, ERR *err)
{
	t_dlist		*start;
	t_dlist		*opd_stack;
	t_tk_type	type;

	start = polish_not[0];
	opd_stack = NULL;
	type = ((t_tok *)start->content)->type;
	while (!err->err_code && start && type != TK_EOF)
	{
		if (is_operand(type))
			opd_stack = push_to_stack(opd_stack, start);
		else
			opd_stack = calculate_res(opd_stack, type, env, err); // we check whether we have operator type 1 or type 2 inside 
		start = start->next;
		type = start ? ((t_tok *)start->content)->type : 0;
	}
	clear_tokens(polish_not, 0);
	return (check_result(opd_stack, env, err));
}