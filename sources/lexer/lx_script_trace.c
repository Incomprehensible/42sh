/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_script_trace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/23 21:01:01 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char		*expr_in(char *s, t_graph *g, t_stx **tr, t_dlist **tk)
{
	if (g->type != TK_VAR && is_token_here(skip_spaces(s), "break"))
		return (pull_break_cont(s, TK_BREAK, tk));
	else if (g->type != TK_VAR && is_token_here(skip_spaces(s), "continue"))
		return (pull_break_cont(s, TK_CONTIN, tk));
	else if (special_condition(g->patt))
		return (pull_expr1(g->patt, s, tr, tk));
	return (pull_expr2(s, tr, tk));
}

static char	*get_script(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
	char	*tmp;

	if ((tmp = into_the_portal(g, s, tok, tr)) != s && tmp)
		return (tmp);
	return (script_pull(g, s, tr, tok));
}

static char	*pull_empty(t_graph *g, char *s, t_dlist **tok)
{
	if (g && g->type != TK_FI && g->type != TK_DONE && g->type != TK_SEP)
		s = parse_empty(s, g->patt, tok);
	else if (g && (*s == ' ' || *s == '\t'))
		s = parse_empty(s, g->patt, tok);
	return (s);
}

char		*scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
	static short	sig;
	char			*tmp;

	s = g ? pull_empty(g, s, tok) : s;
	if (!s || !g || ((tmp = get_script(g, s, tok, tr)) && tmp == s) || !tmp)
		return ((sig && tmp) ? s : NULL);
	s = pull_empty(g, tmp, tok);
	sig = 1;
	while (*s && (g->type == TK_EXPR || g->type == EX) &&
	((tmp = get_script(g, s, tok, tr)) != s) && tmp)
		s = parse_empty(tmp, g->patt, tok);
	if (!tmp)
		return (NULL);
	if (graph_end(g, s) && (sig = 1))
		return (s);
	if (graph_forward_only(g) && !(sig = 0))
		return (scripts_traverse(g->forward, s, tok, tr));
	else
	{
		if ((tmp = scripts_traverse(g->forward, s, tok, tr)) && tmp == s)
			if ((tmp = scripts_traverse(g->left, s, tok, tr)) && tmp == s)
				if ((tmp = scripts_traverse(g->right, s, tok, tr)) && tmp == s)
					return (NULL);
	}
	return (tmp);
}

char		*parse_scripts(char *str, t_dlist **tok, t_stx **tree, short ind)
{
	static t_graph	*script[4];
	char			*tmp;

	if (!ind)
		str = parse_empty(str, 0x0, tok);
	tmp = str;
	if (!script[0])
	{
		script[0] = if_script_in();
		script[1] = while_script_in();
		script[2] = until_script_in();
		script[3] = for_script_in();
	}
	if (!ind)
		ind = graph_type(str);
	if ((str = scripts_traverse(script[ind], str, tok, tree)) == tmp)
		return (NULL);
	return (str);
}
