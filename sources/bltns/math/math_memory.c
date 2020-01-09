/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 22:29:49 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

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

t_dlist	*clean_op_stack(t_dlist *ops)
{
	del_tokens(ops);
	ops = NULL;
	return (ops);
}

t_dlist	*lst_to_end(t_dlist *stack)
{
	while (stack && stack->next)
		stack = stack->next;
	return (stack);
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
