/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_operands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 01:46:38 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

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
			return ((long)error_process(value->txt, value, STR_OPERAND, err));
		res = pull_from_base(value->txt, type);
		dstr_del(&value);
		return (res);
	}
	return (ft_atoi_base(operand->value, (int)operand->type));
}

long	pull_from_base(char *value, t_tk_type type)
{
	long res;

	if (type == HEX || type == BIN)
		value += 2;
	else if (type == SEV)
		value += (*(value + 1) == 'o') ? 2 : 1;
	res = ft_atoi_base(value, type);
	return (res);
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
