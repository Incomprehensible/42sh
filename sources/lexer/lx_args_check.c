/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_args_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/01 03:36:25 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static short	vector_to_left(t_dlist *token_list)
{
	while (token_list && (TOK_TYPE == TK_EMPTY))
		token_list = token_list->prev;
	if (!token_list)
		return (1);
	if (token_list && (TOK_TYPE != TK_SEP && TOK_TYPE != TK_OR
	&& TOK_TYPE != TK_AND && TOK_TYPE != TK_PIPE && TOK_TYPE != TK_BCKR_PS))
		return (0);
	return (1);
}

static short	vector_to_right(t_dlist *token_list)
{
	while (token_list && (TOK_TYPE == TK_EMPTY))
		token_list = token_list->next;
	if (!token_list)
		return (1);
	if (token_list && (TOK_TYPE != TK_SEP && TOK_TYPE != TK_OR
	&& TOK_TYPE != TK_AND && TOK_TYPE != TK_PIPE && TOK_TYPE != TK_ARSHLOCH))
		return (0);
	return (1);
}

short			args_check(t_dlist *token_list)
{
	while (token_list)
	{
		while (token_list && TOK_TYPE == TK_EMPTY)
			token_list = token_list->next;
		if (token_list && (TOK_TYPE == TK_MATH || TOK_TYPE == TK_SUBSH))
		{
			if (!vector_to_left(token_list->prev))
				return (0);
			if (!vector_to_right(token_list->next))
				return (0);
		}
		token_list = token_list ? token_list->next : NULL;
	}
	return (1);
}
