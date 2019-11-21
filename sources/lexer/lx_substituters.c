/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_substituters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/21 21:53:52 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

void	expr_to_value(t_dlist *token_list)
{
	while (token_list && (TOK_TYPE == TK_EMPTY || TOK_TYPE == TK_SEP))
		token_list = token_list->prev;
	while (token_list && (TOK_TYPE != TK_EMPTY && TOK_TYPE != RETURN))
	{
		if (TOK_TYPE == TK_EXPR)
			TOK_TYPE = TK_VALUE;
		token_list = token_list->prev;
	}
}

void	substitute_value(t_dlist *token_list)
{
	while (token_list && TOK_TYPE != TK_ASSIGM)
	{
		if (TOK_TYPE == TK_EXPR)
			TOK_TYPE = TK_VALUE;
		token_list = token_list->prev;
	}
}

void	exec_on(t_dlist *token_list)
{
	while (token_list)
	{
		if (TOK_TYPE == TK_EXPR)
			if (!ft_strcmp(TOK_VALUE, "exec"))
				TOK_TYPE = TK_EXEC;
		token_list = token_list->next;
	}
}
