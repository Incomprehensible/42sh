/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_polish_reverse.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/19 16:51:41 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"
#include "stdio.h"

short	is_operand_tok(t_tk_type type)
{
	if (type == OPRND || type == HEX || type == BIN || type == DEC || type == SEV)
		return (1);
	return (0);
}

short	get_operator_tok(t_tk_type *ops, t_tk_type type)
{
	short i;
	short j;

	i = 0;
	j = 0;
	while (ops[i * 16])
	{
		while (ops[i * 16 + j])
		{
			if (type == ops[i * 16 + j])
				return (i);
			j++;
		}
		i++;
		j = 0;
	}
	return (0);
}

t_dlist	*lst_to_end(t_dlist *stack)
{
	while (stack && stack->next)
		stack = stack->next;
	return (stack);
}

 t_dlist	*push_to_stack(t_dlist *stack, t_dlist *new_elem)
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
 	if (!start)
 	{
 		stack = ft_dlstnew(NULL, 0);
 		start = stack;
 	}
 	else
 	{
 		stack->next = ft_dlstnew(NULL, 0);
 		stack = stack->next;
 	}
 	stack->content = (t_tok *)malloc(sizeof(t_tok));
 	*((t_tok *)(stack->content)) = token_data;
 	stack->next = NULL;
 	stack->prev = tmp;
 	return (start);
 }

//compares indexes of ops - last from stack and preceding one - inside
short	pop_operator(t_dlist *op_stack, t_tk_type new_tok)
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
	if (new_tok == INTO_BR || ((t_tok *)op_stack->content)->type == INTO_BR)
		return (0);
	id1 = get_operator_tok((t_tk_type *)ops, ((t_tok *)op_stack->content)->type);
	id2 = get_operator_tok((t_tk_type *)ops, new_tok);
	if (!id1 && id1 == id2)
		return (0);
	if (id1 < id2 || id1 == id2)
		return (id1 < id2 ? 1 : 2);
	return (0);
}

void	del_tokens(t_dlist *token)
{
	t_dlist *token_list;

	while (token)
    {
		token_list = token;
        token = token->next;
        if (token_list->content && TOK_VALUE)
            free(TOK_VALUE);
		free(token_list->content);
        free(token_list);
    }
}

static short	is_bracket(t_tk_type type)
{
	if (type == INTO_BR || type == OUT_BR)
		return (1);
	return (0);
}

static short	stop_token(t_tk_type stop, t_tk_type current)
{
	if (stop != TK_EOF)
		if (current == INTO_BR)
			return (1);
	return (0);
}

t_dlist	*clean_op_stack(t_dlist *ops)
{
	del_tokens(ops);
	ops = NULL;
	return (ops);
}

t_dlist	*prepare_op_stack(t_dlist *ops, t_tk_type op)
{
	t_dlist		*tmp;

	if (op != OUT_BR)
	{
		tmp = ops;
		ops = (ops && ops->next) ? ops->next : NULL;
	}
	else
		tmp = ops ? ops->prev : NULL;
	del_tokens(ops);
	if (tmp)
	{
		tmp->next = NULL;
		while (tmp->prev)
			tmp = tmp->prev;
	}
	return (tmp);
}

//it clears two stacks inside 
//if we got closing bracket or operand lower value appeared we make or update final linked list
t_dlist	*update_fin_list(t_dlist **fin_list, t_dlist *opds, t_dlist *ops, t_tk_type op)
{
	t_dlist		*start_opds;
	t_dlist		*tmp;

	start_opds = opds;
	while (opds)
	{
		fin_list[0] = push_to_stack(fin_list[0], opds);
		opds = opds->next;
	}
	ops = lst_to_end(ops);
	while (ops && !stop_token(op, ((t_tok *)ops->content)->type))
	{
		if (!is_bracket(((t_tok *)ops->content)->type))
			fin_list[0] = push_to_stack(fin_list[0], ops);
		if (!ops->prev)
			break ;
		ops = ops->prev;
	}
	tmp = lst_to_end(fin_list[0]);
	fin_list[1] = tmp;
	del_tokens(start_opds);
	if (ops && !ops->prev)
		return (clean_op_stack(ops));
	else
		return (prepare_op_stack(ops, op));
	// if (op != OUT_BR)
	// {
	// 	tmp = ops;
	// 	ops = (ops && ops->next) ? ops->next : NULL;
	// }
	// else
	// 	tmp = ops ? ops->prev : NULL;
	// del_tokens(ops);
	// if (tmp)
	// {
	// 	tmp->next = NULL;
	// 	while (tmp->prev)
	// 		tmp = tmp->prev;
	// }
	// //del_tokens(ops);
	// return (tmp);
}

t_dlist	*into_fin_list(t_dlist **fin_list, t_dlist *opds, t_dlist *ops)
{
	t_dlist		*start_opds;
	t_dlist		*tmp;

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

t_dlist	*go_through_brackets(t_dlist *dimon_loh, t_tk_type type)
{
	while (dimon_loh && (type == OUT_BR || type == TK_EOF))
	{
		dimon_loh = dimon_loh->next;
		type = dimon_loh ? ((t_tok *)dimon_loh->content)->type : 0;
	}
	return (dimon_loh);
}

void	into_reverse_notation(t_dlist *dimon_loh, t_dlist **fin)
{
	t_tk_type		type;
	t_dlist			*op_stack;
	t_dlist			*opd_stack;
	int				id;

	id = 0;
	op_stack = NULL;
	opd_stack = NULL;
	while (dimon_loh)
	{
		type = ((t_tok *)dimon_loh->content)->type;
		if (is_operand_tok(type))
			opd_stack = push_to_stack(opd_stack, dimon_loh); //add new operand to stack
		else if (type == TK_EOF || (id = pop_operator(op_stack, type)))
		{
			op_stack = id < 2 ? update_fin_list(fin, opd_stack, op_stack, type) : into_fin_list(fin, opd_stack, op_stack);
			opd_stack = NULL;
			dimon_loh = go_through_brackets(dimon_loh, type);
			continue ;
		}
		else
			op_stack = push_to_stack(op_stack, dimon_loh); //add new operator or bracket to stack
		dimon_loh = dimon_loh->next;
	}
	if (op_stack)
		update_fin_list(fin, opd_stack, op_stack, TK_EOF);
}

long	ariphmetic_calc(t_dlist **dimon_loh, ENV *env, ERR *err)
{
	t_dlist				*polish_not[2];
	long 				pshe_pshe_res;

	ft_bzero(polish_not, sizeof(t_dlist *) * 2);
	//DBG_PRINT_MATH(dimon_loh[0]);
	into_reverse_notation(dimon_loh[0], polish_not);
	clear_tokens(dimon_loh, 0);
	if (err->err_code)
		return (0);
	if (!polish_not[0])
		return (0);
	DBG_PRINT_MATH(polish_not[0]);
	pshe_pshe_res = polish_calculate(polish_not, env, err);
	return (pshe_pshe_res);
}