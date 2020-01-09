/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_obliczac.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 03:04:12 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"
#define OPD_TYPE ((t_tok *)op_stack->content)->type

static t_dlist	*push_elem(t_dlist *start, t_dlist *stack)
{
	if (!start)
		stack = ft_dlstnew(NULL, 0);
	else
	{
		stack->next = ft_dlstnew(NULL, 0);
		stack = stack->next;
	}
	return (stack);
}

t_dlist			*push_to_stack(t_dlist *stack, t_dlist *new_elem)
{
	t_dlist		*start;
	t_dlist		*tmp;
	char		*value;
	t_tok		token_data;

	value = ((t_tok *)new_elem->content)->value;
	token_data.value = (value) ? ft_strdup(value) : NULL;
	token_data.type = ((t_tok *)new_elem->content)->type;
	start = stack;
	stack = lst_to_end(stack);
	tmp = stack;
	stack = push_elem(start, stack);
	start = start ? start : stack;
	stack->content = (t_tok *)malloc(sizeof(t_tok));
	*((t_tok *)(stack->content)) = token_data;
	stack->next = NULL;
	stack->prev = tmp;
	return (start);
}

short			pop_operator(t_dlist *op_stack, t_tk_type new_tok)
{
	static t_tk_type	ops[7][16];
	short				id1;
	short				id2;

	if (!op_stack)
		return (0);
	if (!ops[0][0])
		ops_init((t_tk_type *)ops);
	if (new_tok == OUT_BR)
		return (1);
	op_stack = lst_to_end(op_stack);
	if (new_tok == INTO_BR || OPD_TYPE == INTO_BR)
		return (0);
	id1 = get_operator_tok((t_tk_type *)ops, OPD_TYPE);
	id2 = get_operator_tok((t_tk_type *)ops, new_tok);
	if (!id1 && id1 == id2)
		return (0);
	if (id1 < id2 || id1 == id2)
		return (id1 < id2 ? 1 : 2);
	return (0);
}
