/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_checkers_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/22 15:13:06 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short	back_ps_check(t_dlist *token_list)
{
	t_tk_type	type;

	while (token_list)
	{
		while (token_list && TOK_TYPE != TK_ARSHLOCH)
		{
			type = (TOK_TYPE == TK_EMPTY) ? type : TOK_TYPE;
			token_list = token_list->next;
		}
		if (token_list && (type == TK_SEP || type == TK_AND ||
		type == TK_OR || type == TK_PIPE))
			return (0);
		if (token_list)
			token_list = token_list->next;
		while (token_list && TOK_TYPE == TK_EMPTY)
			token_list = token_list->next;
		if (!token_list)
			return (1);
		else if (TOK_TYPE != TK_SEP)
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

short	tok_follw(t_dlist *token_list)
{
	if (!token_list || !token_list->next)
		return (0);
	token_list = token_list->next;
	while (token_list && TOK_TYPE == TK_EMPTY)
		token_list = token_list->next;
	if (!token_list || is_sep_token(TOK_TYPE))
		return (0);
	return (1);
}

short	n_after_n(char *prev_sep, char sep)
{
	if (!prev_sep || !sep)
		return (0);
	else if (*prev_sep == '\n' && sep == '\n')
		return (1);
	return (0);
}

short	check_sep_cond(t_dlist *token_list, t_tk_type type)
{
	if (!token_list && type == TK_BCKR_PS)
		return (0);
	if (token_list && !token_list->next && TOK_TYPE == TK_BCKR_PS)
		return (0);
	if (token_list && is_sep_token(TOK_TYPE) && is_sep_token((type)))
		return (0);
	return (1);
}

short	seps_check(t_dlist *token_list)
{
	short	flag;

	flag = 0;
	while (token_list)
	{
		while (token_list && TOK_VALUE && *TOK_VALUE == '\n')
		{
			flag = 0;
			token_list = token_list->next;
		}
		while (token_list && TOK_TYPE == TK_EMPTY)
			token_list = token_list->next;
		if (token_list && is_sep_token(TOK_TYPE) && TOK_TYPE != TK_BCKR_PS)
		{
			if (!flag)
				return (0);
			flag = 0;
		}
		else if (token_list && TOK_TYPE == TK_BCKR_PS && !tok_follw(token_list))
			return (0);
		else
			flag = 1;
		token_list = (token_list) ? token_list->next : token_list;
	}
	return (1);
}
