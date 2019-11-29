/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_redir_trace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 00:25:10 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static short	stop_point(t_dlist **to_list, char **s)
{
	t_tk_type	tk;
	t_dlist		*token_list;

	tk = 0;
	token_list = to_list[1];
	while (token_list && TOK_TYPE == TK_EMPTY)
		token_list = token_list->prev;
	if (token_list)
		tk = TOK_TYPE;
	if (is_tok_redir(tk, 1) || tk == TK_EXPR)
		return (0);
	if (is_sep_no_space(**s))
		*s = parse_sep(*s, to_list, 0);
	return (1);
}

static short	prof_found(t_tk_type type, t_dlist *token_list)
{
	while (token_list && TOK_TYPE == TK_EMPTY)
		token_list = token_list->prev;
	if (token_list && (TOK_TYPE == TK_PROF_IN || TOK_TYPE == TK_PROF_OUT))
		if (is_tok_redir(type, 1))
			return (1);
	return (0);
}

char			*redir_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
	char			*tmp;
	static short	sig;

	s = g ? parse_empty(s, g->patt, tok) : s;
	if (!g || ((tmp = redirect_pull(g, s, tr, tok)) && tmp == s) || !tmp)
		return (sig ? tmp : NULL);
	if (prof_found(g->type, tok[1]))
		g = g->forward;
	s = tmp;
	if ((sig = 1) && *s != '\n')
		s = parse_empty(s, g->patt, tok);
	if (stop_point(tok, &s))
		return (s);
	if (graph_forward_only(g) && !(sig = 0))
		return (redir_traverse(g->forward, s, tok, tr));
	else
	{
		if ((tmp = redir_traverse(g->forward, s, tok, tr)) && tmp == s)
			if ((tmp = redir_traverse(g->left, s, tok, tr)) && tmp == s)
			{
				sig = 0;
				return (redir_traverse(g->right, s, tok, tr));
			}
	}
	return (tmp);
}

static short	is_fd(char *str)
{
	if (!(*str >= 48 && *str <= 57))
		return (0);
	while (*str && ft_isdigit(*str))
		str++;
	if (*str == '<' || *str == '>')
		return (1);
	return (0);
}

char			*parse_redir(char *str, t_dlist **tok, t_stx **tree, short i)
{
	t_graph	*redir;
	char	*tmp;

	i = 0;
	str = parse_empty(str, 0x0, tok);
	tmp = str;
	redir = redir_in();
	if (*str == '&' || ((*str == '<' || *str == '>') && *(str + 1) == '&'))
		redir = redir->left;
	else if (*str == '<' || *str == '>')
		redir = redir->right;
	else if (is_fd(str))
		redir = redir->forward;
	if ((str = redir_traverse(redir, str, tok, tree)) == tmp)
		return (block_pass(EXPRS, str, tok, tree));
	if (!str)
		return (NULL);
	return (str);
}
