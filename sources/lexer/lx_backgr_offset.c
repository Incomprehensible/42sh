/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_backgr_offset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/21 23:46:15 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short		is_flow(t_tk_type type)
{
	if (type == TK_WHILE || type == TK_UNTIL || type == TK_FOR)
		return (1);
	return (0);
}

static void	all_to_backgr(t_dlist **token, t_dlist *backgr)
{
	t_dlist *first;

	first = token[0];
	first->prev = backgr;
	backgr->next = first;
	backgr->prev = NULL;
	token[0] = backgr;
	token[1]->next = NULL;
}

static void	to_backgr(t_dlist *token_list, t_dlist *backgr)
{
	t_dlist *tmp;

	tmp = token_list->next;
	token_list->next = backgr;
	backgr->prev = token_list;
	backgr->next = tmp;
	if (tmp && tmp->prev)
		tmp->prev = backgr;
}

short		stop_point(t_tk_type type)
{
	if (type != TK_SEP && type != TK_BCKR_PS && type != TK_ARSHLOCH)
		return (0);
	return (1);
}

t_tk_type	make_offset(t_dlist **token, t_tk_type type)
{
	t_dlist *token_list;
	t_dlist *backgr;

	make_token(token, NULL, type);
	token_list = token[1]->prev;
	backgr = token[1];
	while (token_list && !stop_point(TOK_TYPE) && token_list->prev)
	{
		if (TOK_TYPE == TK_FI)
			token_list = skip_if_script(token_list);
		else if (TOK_TYPE == TK_DONE)
			token_list = skip_done_script(token_list);
		else if (TOK_TYPE == TK_FEND)
			token_list = skip_function(token_list);
		token_list = (token_list->prev) ? token_list->prev : token_list;
	}
	token[1] = backgr->prev;
	if (!token_list || !token_list->prev)
		all_to_backgr(token, backgr);
	else
		to_backgr(token_list, backgr);
	make_token(token, NULL, TK_ARSHLOCH);
	return (TK_BCKR_PS);
}
