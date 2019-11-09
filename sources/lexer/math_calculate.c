/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_calculate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/09 05:18:52 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

static short	is_operand(t_tk_type type)
{
	if (type == OPRND || type == DEC || type == HEX || type == BIN || type == SEV)
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

//the fuck you don't have a func that only counts this shitty list
long	check_result(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char	*value;
	DSTRING	*tmp;
	long	res;

	res = 0;
	if (!err->err_code)
	{
		value = opd_stack ? ((t_tok *)opd_stack->content)->value : NULL;
		if (!value || (count_dlist(opd_stack)) != 1)
		{
			set_error(value, OPERAND_EXP, err);
			return (0);
		}
		if (((t_tok *)opd_stack->content)->type == OPRND)
		{
			tmp = env_get_variable(value, env);
			res = ft_atoi(tmp->txt);
			dstr_del(&tmp);
		}
		else
			res = ft_atoi_base(value, ((t_tok *)opd_stack->content)->type);
			// res = ft_atoi(value);
	}
	del_tokens(opd_stack);
	return (res);
}

t_tk_type	is_number(char *value)
{
	t_tk_type type;

	if (layer_parse_two("0x@01@", value))
		type = BIN;
	else if (layer_parse_two("0x@0123456789abcdefABCDEF@", value))
		type = HEX;
	else if (layer_parse_two("0@01234567@", value))
		type = SEV;
	else if (layer_parse_two("0o@01234567@", value))
		type = SEV;
	else if (!(*value == '0' && *(value + 1)) && layer_parse_two("@0123456789@", value))
		type = DEC;
	else
		return (0);
	return (type);
}

static short	get_op_type(t_tk_type op)
{
	if (op == INCRM || op == DECRM || op == POSIT || op == NEGAT || op == REJECT || op == NOT)
		return (1);
	// else if (op == MORE || op == LESS || op == IS_EQU || op == NO_EQU || op == LESS_EQ || op == MORE_EQ)
	// 	return (3);
	else if (op == EQU)
		return (3);
	return (2);
}

void	compare_level_in(t_tk_type *arr)
{
	arr[0] = IS_EQU;
	arr[1] = NO_EQU;
	arr[2] = LESS_EQ;
	arr[3] = MORE_EQ;
	arr[4] = LESS;
	arr[5] = MORE;
	arr[2] = 0;
}

void	normal_level_in(t_tk_type *arr)
{
	arr[0] = PLUS;
	arr[1] = MINUS;
	arr[2] = MULT;
	arr[3] = DEVID;
	arr[4] = DEVREM;
	arr[5] = 0;
}

void	bit_level_in(t_tk_type *arr)
{
	arr[0] = BIT_R;
	arr[1] = BIT_L;
	arr[2] = AND;
	arr[3] = OR;
	arr[4] = XOR;
	arr[5] = 0;
}

void	logic_level_in(t_tk_type *arr)
{
	arr[0] = LOG_AND;
	arr[1] = LOG_OR;
	arr[2] = 0;
}

void	assign_level_in(t_tk_type *arr)
{
	arr[0] = EQU;
	arr[1] = PLUS_EQ;
	arr[2] = MIN_EQ;
	arr[3] = 0;
}

static t_tk_type	get_ind(t_tk_type op)
{
	static t_tk_type	*ops[5];
	t_tk_type			type;
	t_tk_type			i;

	if (!ops[0])
	{
		ops[0] = (t_tk_type *)ft_memalloc(sizeof(t_tk_type) * 6);
		ops[1] = (t_tk_type *)ft_memalloc(sizeof(t_tk_type) * 6);
		ops[2] = (t_tk_type *)ft_memalloc(sizeof(t_tk_type) * 4);
		ops[3] = (t_tk_type *)ft_memalloc(sizeof(t_tk_type) * 3);
		ops[4] = (t_tk_type *)ft_memalloc(sizeof(t_tk_type) * 4);
		bit_level_in(ops[0]);
		normal_level_in(ops[1]);
		compare_level_in(ops[2]);
		logic_level_in(ops[3]);
		assign_level_in(ops[4]);
	}
	type = 0;
	i = 0;
	while (ops[type])
	{
		while (ops[type][i] && ops[type][i] != op)
			i++;
		if (ops[type][i] == op)
			return (type);
		type++;
		i = 0;
	}
	return (type);
}

// t_dlist	*substitute_single(t_dlist *opd_stack, ENV *env, long res, t_tk_type op)
// {
// 	char	*value;
// 	t_dlist	*tmp;

// 	value = ft_itoa(res);
// 	if (((t_tok *)opd_stack->content)->type == OPRND && (op == INCRM || op == DECRM))
// 	{
// 		env_unset_variable(((t_tok *)tmp->content)->value, env);
// 		env_set_variable(((t_tok *)opd_stack->content)->value, dstr_new(value), env);
// 		free(value);
// 	}
// 	else
// 	{
// 		free(((t_tok *)opd_stack->content)->value);
// 		((t_tok *)opd_stack->content)->value = value;
// 		((t_tok *)opd_stack->content)->type = DEC;
// 	}
// 	while (opd_stack && opd_stack->prev)
// 		opd_stack = opd_stack->prev;
// 	return (opd_stack);
// }

t_dlist	*substitute_single(t_dlist *opd_stack, ENV *env, long res, t_tk_type op)
{
	char	*value;
	t_dlist	*tmp;

	value = ft_itoa(res);
	if (((t_tok *)opd_stack->content)->type == OPRND && (op == INCRM || op == DECRM))
		env_set_variable(((t_tok *)opd_stack->content)->value, dstr_new(value), env);
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

	value = ft_itoa(res);
	tmp = opd_stack->prev;
	del_tokens(opd_stack);
	tmp->next = NULL;
	if (((t_tok *)tmp->content)->type == OPRND && (op == PLUS_EQ || op == MIN_EQ))
		env_set_variable(((t_tok *)tmp->content)->value, dstr_new(value), env);
	free(((t_tok *)tmp->content)->value);
	((t_tok *)tmp->content)->value = value;
	((t_tok *)tmp->content)->type = DEC;
	opd_stack = tmp;
	while (opd_stack && opd_stack->prev)
		opd_stack = opd_stack->prev;
	return (opd_stack);
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

long	fetch_operand(t_tok *operand,  ENV *env, ERR *err)
{
	char		*value;
	t_tk_type	type;
	long		res;

	if (operand->type == OPRND)
	{
		value = (env_get_variable(operand->value, env))->txt;
		if (!value || !(*value))
		{
			if (value)
				free(value);
			value = ft_strdup("0");
			type = 10;
		}
		else if (!(type = is_number(value)))
		{
			set_error(value, STR_OPERAND, err);
			return (0);
		}
		res = pull_from_base(value, type);
		free(value);
		return (res);
	}
	return (ft_atoi_base(operand->value, (int)operand->type));
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
	{
		if (opd_stack)
		{
			set_error(((t_tok *)opd_stack->content)->value, OPERAND_EXP, err);
			return (opd_stack);
		}
		else
			return (set_error(NULL, OPERAND_EXP, err));
	}
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
	char		*value;
	t_tk_type	type;
	long		res;
	
	new = ((t_tok *)opd_stack->content)->value;
	value = env_get_variable(((t_tok *)opd_stack->content)->value, env)->txt;
	if (!value)
		value = ft_strdup("0");
	if (!(type = is_number(value)))
	{
		set_error(new, STR_OPERAND, err);
		free(value);
		return (NULL);
	}
	res = ft_atoi_base(value, type);
	new = ((t_tok *)opd_stack->prev->content)->value;
	env_unset_variable(new, env);
	free(value);
	value = ft_itoa(res);
	env_set_variable(new, dstr_new(value), env);
	return (value);
}

char *set_var(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char		*value;
	char		*new;
	t_tk_type	type;
	long		res;
	
	value = ((t_tok *)opd_stack->content)->value;
	type = ((t_tok *)opd_stack->content)->type;
	res = ft_atoi_base(value, type);
	new = ft_itoa(res);
	value = ((t_tok *)opd_stack->prev->content)->value;
	env_unset_variable(value, env);
	env_set_variable(value, dstr_new(new), env);
	return (new);
}

//if we equal something, we like normal substitute opd to new assigned value
t_dlist	*set_new_var(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char		*new;
	t_dlist		*new_elem;
	char		*value;
	//t_tok		*token;

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
	char		*tmp;

	if (!opd_stack || !opd_stack->prev)
	{
		if (opd_stack)
		{
			set_error(((t_tok *)opd_stack->content)->value, OPERAND_EXP, err);
			return (opd_stack);
		}
		else
			return (set_error(NULL, OPERAND_EXP, err));
	}
	opd = ((t_tok *)opd_stack->prev->content)->type;
	value = ((t_tok *)opd_stack->prev->content)->value;
	if (opd != OPRND)
	{
		set_error(value, INVALID_ASSIG, err);
		return (opd_stack);
	}
	return (set_new_var(opd_stack, env, err));
}

t_dlist	*calculate_res(t_dlist *opd_stack, t_tk_type op, ENV *env, ERR *err)
{
	short	id;
	t_dlist	*start;

	start = opd_stack;
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

t_dlist	*push_operand(t_dlist *opd_stack, t_dlist *current, ENV *env, ERR *err)
{
	t_tk_type	type;
	char		*value;

	type = ((t_tok *)current->content)->type;
	if (type == OPRND)
	{
		value = env_get_variable(((t_tok *)current->content)->value, env)->txt;
		if (!value)
			value = ft_strdup("0");
		else if (!is_number(value))
		{
			set_error(value, STR_OPERAND, err);
			return (opd_stack);
		}
		free(((t_tok *)current->content)->value);
		((t_tok *)current->content)->value = value;
	}
	return (push_to_stack(opd_stack, current));
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
		else if (type != TK_EOF)
			opd_stack = calculate_res(opd_stack, type, env, err); // we check whether we have operator type 1 or type 2 inside 
		start = start->next;
		type = start ? ((t_tok *)start->content)->type : 0;
	}
	clear_tokens(polish_not, 0);
	return (check_result(opd_stack, env, err));
}


// t_dlist	*get_single_opd(t_dlist *opd_stack, t_tk_type op, ERR *err)
// {
// 	static long			(*ptr[6])(long);
// 	long 				res;
// 	long				a;

// 	if (!ptr[0])
// 	{
// 		ptr[0] = increment_opd();
// 		ptr[1] = decrement_opd();
// 		ptr[2] = reject_opd();
// 		ptr[3] = positive_opd();
// 		ptr[4] = negative_opd();
// 		ptr[5] = make_none_opd();
// 	}
// 	if (!opd_stack)
// 		return (set_error(NULL, OPERAND_EXP, err));
// 	a = ft_atoi_base(((t_tok *)opd_stack->content)->value, (int)((t_tok *)opd_stack->content)->type);
// 	res = ptr[get_ind(op)](a);
// 	return (substitute_value(opd_stack, res, 1));
// }

// t_dlist	*get_both_opd(t_dlist *opd_stack, t_tk_type op, ERR *err)
// {
// 	static long			(*ptr[6])(long, long);
// 	long 				res;
// 	long				a;
// 	long				b;

// 	if (!ptr[0])
// 	{
// 		ptr[0] = add_opd();
// 		ptr[1] = subst_opd();
// 		ptr[2] = mult_opd();
// 		ptr[3] = devid_opd();
// 		ptr[4] = devidrem_opd();
// 		ptr[5] = compare_opd();
// 	}
// 	if (!opd_stack || !opd_stack->prev)
// 	{
// 		if (opd_stack)
// 		{
// 			set_error(((t_tok *)opd_stack->content)->value, OPERAND_EXP, err);
// 			return (opd_stack);
// 		}
// 		else
// 			return (set_error(NULL, OPERAND_EXP, err));
// 	}
// 	a = ft_atoi_base(((t_tok *)opd_stack->prev->content)->value, (int)((t_tok *)opd_stack->prev->content)->type);
// 	b = ft_atoi_base(((t_tok *)opd_stack->content)->value, (int)((t_tok *)opd_stack->content)->type);
// 	res = ptr[get_ind(op)](a, b);
// 	return (substitute_value(opd_stack, res, 2));
// }

// static t_tk_type	get_ind(t_tk_type op)
// {
// 	static t_tk_type	ops[] = {
// 	INCRM, DECRM, NEGAT,
// 	REJECT, NOT, POSIT, NULL
// 	};
// 	t_tk_type			type;

// 	type = 0;
// 	while (ops[type] != op)
// 		type++;
// 	return (type);
// }

// t_dlist	*push_operand(t_dlist *opd_stack, t_dlist *current, ENV *env, ERR *err)
// {
// 	t_tk_type	type;
// 	char		*value;

// 	type = ((t_tok *)current->content)->type;
// 	if (type == OPRND)
// 	{
// 		value = env_get_variable(((t_tok *)current->content)->value, env);
// 		if (!value)
// 			value = ft_strdup("0");
// 		else if (!is_number(value, current))
// 		{
// 			set_error(value, STR_OPERAND, err);
// 			return (opd_stack);
// 		}
// 		free(((t_tok *)current->content)->value);
// 		((t_tok *)current->content)->value = value;
// 	}
// 	return (push_to_stack(opd_stack, current));
// }

// t_dlist	*substitute_both_value(t_dlist *opd_stack, ENV *env, long res, t_tk_type op)
// {
// 	char	*value;
// 	t_dlist	*tmp;

// 	value = ft_itoa(res);
// 	tmp = opd_stack->prev;
// 	del_tokens(opd_stack);
// 	tmp->next = NULL;
// 	if (((t_tok *)tmp->content)->type == OPRND)
// 	{
// 		env_set_variable(((t_tok *)tmp->content)->value, dstr_new(value), env);
// 		free(value);
// 	}
// 	else
// 	{
// 		free(((t_tok *)tmp->content)->value);
// 		((t_tok *)tmp->content)->value = value;
// 		((t_tok *)tmp->content)->type = DEC;
// 		opd_stack = tmp;
// 	}
// 	while (opd_stack)
// 		opd_stack = opd_stack->prev;
// 	return (opd_stack);
// }