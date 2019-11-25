/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_script_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 18:30:09 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static char		*pull_math(char *s, t_dlist **tok, t_stx **tr, short tp)
{
	if (!layer_parse_two("((w))", s))
		return (s);
	if (tp == FORMATH && !validate_triple_struct(s, 0))
		return (0);
	else if (tp == MATH_NT && !validate_simple_struct(s, 0))
		return (0);
	while (ft_isspace(*s))
		s = (*s == '\\') ? s + 2 : ++s;
	s = pull_legit_math(s, tok, tr);
	return (s);
}

static char		*make_it_glue(char *s, t_stx **tr, t_dlist **tok)
{
	char	*tmp;

	tmp = parse_str_block(s, tok, tr, '\n');
	if (!tmp || tmp == s)
		return (NULL);
	if (!sep_detected(tok[1]) || !check_valid_sep(tok[1]))
		return (NULL);
	return (tmp);
}

static char		*normal_token(char *patt, t_tk_type tp, char *s, t_dlist **tok)
{
	size_t	i;
	size_t	space;

	space = 0;
	if ((i = layer_parse_two(patt, s)))
	{
		if (ft_isspace(*(s + i - 1)))
			space = remove_spaces(s + i - 1, i);
		make_token(tok, NULL, tp);
		s = s + i;
	}
	return (s);
}

char		*pull_break_cont(char *s, t_tk_type tp, t_dlist **tok)
{
	char	*tmp;

	tmp = NULL;
	if (tp == TK_BREAK)
		tmp = normal_token("break_", TK_BREAK, s, tok);
	else if (tp == TK_CONTIN)
		tmp = normal_token("continue_", TK_CONTIN, s, tok);
	if (tmp == s)
		return (s);
	s = tmp;
	if (*s == ' ' || *s == '\t')
		s = parse_empty(s, 0x0, tok);
	s = parse_sep(s, tok, 0);
	if (!sep_detected(tok[1]) || !check_valid_sep(tok[1]))
		return (NULL);
	return (s);
}

char			*script_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
	if (!(*s))
		return (s);
	if (g->type == TK_SEP)
		return (parse_sep(s, tok, 0));
	if (g->type == GLUE)
		return (make_it_glue(s, tr, tok));
	if (g->type == TK_EXPR || g->type == TK_VAR || g->type == EX)
		return (expr_in(s, g, tr, tok));
	else if (g->type == MATH_NT || g->type == FORMATH)
		return (pull_math(s, tok, tr, g->type));
	else
		s = normal_token(g->patt, g->type, s, tok);
	if (*s == ' ' || *s == '\t')
		s = parse_empty(s, 0x0, tok);
	if (is_sep_no_space(*s) && (g->type != TK_DONE && g->type != TK_FI))
		s = parse_sep(s, tok, 0);
	return (s);
}
