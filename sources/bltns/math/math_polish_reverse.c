/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_polish_reverse.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 02:48:45 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

t_dlist	*upd_finlst(t_dlist **fin, t_dlist *opds, t_dlist *ops, t_tk_type op)
{
	t_dlist	*start_opds;
	t_dlist	*tmp;

	start_opds = opds;
	while (opds)
	{
		fin[0] = push_to_stack(fin[0], opds);
		opds = opds->next;
	}
	ops = lst_to_end(ops);
	while (ops && !stop_token(op, ((t_tok *)ops->content)->type))
	{
		if (!is_bracket(((t_tok *)ops->content)->type))
			fin[0] = push_to_stack(fin[0], ops);
		if (!ops->prev)
			break ;
		ops = ops->prev;
	}
	tmp = lst_to_end(fin[0]);
	fin[1] = tmp;
	del_tokens(start_opds);
	if (ops && !ops->prev)
		return (clean_op_stack(ops));
	else
		return (prepare_op_stack(ops, op));
}

t_dlist	*into_finlst(t_dlist **fin_list, t_dlist *opds, t_dlist *ops)
{
	t_dlist	*start_opds;
	t_dlist	*tmp;

	start_opds = opds;
	while (opds)
	{
		fin_list[0] = push_to_stack(fin_list[0], opds);
		opds = opds->next;
	}
	ops = lst_to_end(ops);
	fin_list[0] = push_to_stack(fin_list[0], ops);
	tmp = fin_list[0];
	tmp = lst_to_end(tmp);
	fin_list[1] = tmp;
	del_tokens(start_opds);
	tmp = ops ? ops->prev : NULL;
	if (tmp)
	{
		tmp->next = NULL;
		while (tmp->prev)
			tmp = tmp->prev;
	}
	del_tokens(ops);
	return (tmp);
}

void	into_reverse_notation(t_dlist *dimon_loh, t_dlist **fin, int id)
{
	t_tk_type	type;
	t_dlist		*op_stack;
	t_dlist		*opd_stack;

	op_stack = NULL;
	opd_stack = NULL;
	while (dimon_loh)
	{
		type = ((t_tok *)dimon_loh->content)->type;
		if (is_operand_tok(type))
			opd_stack = push_to_stack(opd_stack, dimon_loh);
		else if (type == TK_EOF || (id = pop_operator(op_stack, type)))
		{
			op_stack = id < 2 ? upd_finlst(fin, opd_stack, op_stack, type) : \
			into_finlst(fin, opd_stack, op_stack);
			opd_stack = NULL;
			dimon_loh = go_through_brackets(dimon_loh, type);
			continue ;
		}
		else
			op_stack = push_to_stack(op_stack, dimon_loh);
		dimon_loh = dimon_loh->next;
	}
	if (op_stack)
		upd_fin_list(fin, opd_stack, op_stack, TK_EOF);
}

long	ariphmetic_calc(t_dlist **dimon_loh, ENV *env, ERR *err)
{
	t_dlist	*polish_not[2];
	long	pshe_pshe_res;

	ft_bzero(polish_not, sizeof(t_dlist *) * 2);
	into_reverse_notation(dimon_loh[0], polish_not, 0);
	clear_tokens(dimon_loh, 0);
	if (err->err_code)
		return (0);
	if (!polish_not[0])
		return (0);
	pshe_pshe_res = polish_calculate(polish_not, env, err);
	return (pshe_pshe_res);
}
