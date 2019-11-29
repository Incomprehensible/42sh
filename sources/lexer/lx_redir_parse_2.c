/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_redir_parse_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 00:18:27 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short			is_tok_redir(t_tk_type type, short id)
{
	if (!id)
	{
		if (type == TK_PROF_OUT || type == TK_PROF_IN || type == TK_RD_A ||
			type == TK_RD_RW || type == TK_RD_W || type == TK_RD_R ||
			type == TK_FD || type == TK_FILENAME)
			return (1);
		return (0);
	}
	if (type == TK_RD_A || type == TK_RD_RW || type == TK_RD_W
	|| type == TK_RD_R)
		return (1);
	return (0);
}

static size_t	parse_fd(char *meta, char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && *meta && (str[i] == *meta || special_meta(*meta)))
	{
		if (*meta && special_meta(*meta))
			return (decimals_proceed(str, meta));
		else
		{
			str++;
			meta++;
			i++;
		}
		if (!(*meta))
			return (i);
	}
	return (*meta ? 0 : i);
}

char			*fd_pull(t_graph *g, char *s, t_dlist **tok)
{
	short	i;
	char	*new;

	while (g && !(i = parse_fd(g->patt, s)))
		g = g->next;
	if (i)
	{
		if (g->type == CLOSE)
			new = ft_strdup("-42");
		else
			new = pull_token(s, i);
		make_token(tok, new, TK_FD);
		s = s + i;
	}
	return (s);
}

char			*redir_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
	short	i;

	if (layer_parse_one("<(z)", s) || layer_parse_one(">(z)", s))
		return (parse_proc(s, tok, tr, PROC));
	while (g && !(i = layer_parse_two(g->patt, s)))
		g = g->next;
	if (i && i < 3)
	{
		if (*s == '&')
			make_token(tok, ft_strdup("-21"), TK_FD);
		make_token(tok, NULL, g->type);
		s = s + i;
	}
	return (s);
}

short			redir_proceeded(t_dlist *token_list)
{
	while (token_list && TOK_TYPE == TK_EMPTY)
		token_list = token_list->prev;
	if (token_list && token_list->content && is_tok_redir(TOK_TYPE, 1))
		return (1);
	return (0);
}
