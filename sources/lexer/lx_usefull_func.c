/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_usefull_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/22 01:50:47 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short	is_sep_no_space(char str)
{
	if (str != '\n' && str != ';' && str != '|' &&
	str != '&' && str != '\0')
		return (0);
	return (1);
}

short	is_sep_token(t_tk_type type)
{
	if (type == TK_SEP || type == TK_AND || type == TK_OR
	|| type == TK_BCKR_PS || type == TK_PIPE)
		return (1);
	return (0);
}

short	sep_detected(t_dlist *token_list)
{
	if (!token_list->content || !TOK_TYPE)
		return (0);
	while (token_list && TOK_TYPE == TK_EMPTY)
		token_list = token_list->prev;
	if (!token_list)
		return (0);
	if (TOK_TYPE != TK_SEP)
		return (0);
	return (1);
}

short	graph_end(t_graph *g, char *str)
{
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (g->type == TK_DONE || g->type == TK_FI)
		if (is_token_here(skip_spaces(str), "done") ||
		is_token_here(skip_spaces(str), "fi"))
			return (1);
	if ((g->type == TK_FI || g->type == TK_DONE) && !(*str))
		return (1);
	if (!g->forward && !g->right && !g->left)
		return (1);
	return (0);
}

short	graph_forward_only(t_graph *g)
{
	if (g->forward && (!g->right && !g->left))
		return (1);
	return (0);
}
