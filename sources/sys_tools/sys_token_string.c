/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_token_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 09:49:06 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 07:38:33 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_token_to_str.h"
#include "sys_tools/sys_errors.h"
#include "executer.h"
#include "parser.h"

char		*g_string_toks[] = {
	"<<", " ", 0, "; ", "|| ", "&& ", " ", ">", "<", ">>", "<>", 0, 0, 0, "| ",
	"=", 0, 0, "((   ))", "(  )", "$", "if ", "then ", "else ", "fi ",
	"while ", "do ", "done ", "for ", "in ", "break ", "continue ", "exec ",
	"function ", "{ ", "return ", "\n", "} ", 0, "until ", ">(  ) ", "<(  ) ",
	">(  ) ", "<(  ) ", "& ", "... "
};

DSTRING		*token_get_dstring(t_tok *tok)
{
	int			num;
	size_t		tp;
	char		*res;

	num = 0;
	tp = tok->type;
	while ((tp >>= 1) && num < 45)
		++num;
	if ((res = g_string_toks[num]) == NULL)
		return (NULL);
	return (dstr_new(res));
}

DSTRING		*token_get_dvalue(t_tok *tok, int rd_fl)
{
	DSTRING		*val;

	val = NULL;
	if ((val = token_get_dstring(tok)))
	{
		if (tok->type & (TK_MATH | TK_PROC_IN | TK_PROC_OUT |
		TK_PROF_IN | TK_PROF_OUT))
			dstr_insert_str(val, tok->value, 3);
		else if (tok->type == TK_SUBSH)
			dstr_insert_str(val, tok->value, 2);
		else if (tok->type == TK_ASSIGM && tok->value[0] == '+')
			dstr_insert_ch(val, '+', 0);
		else if ((tok->type & (TK_RD_A | TK_RD_R |
		TK_RD_RW | TK_RD_W)) && !rd_fl)
			dstr_insert_ch(val, '&', val->strlen);
	}
	else if (tok->value && !(val = dstr_new(tok->value)))
		return (NULL);
	else if (!tok->value && !(val = dstr_new("")))
		return (NULL);
	return (val);
}

int			str_from_tok_it(DSTRING *buff, t_dlist *tls, \
t_dlist *end_tk, t_tk_type endtk)
{
	DSTRING *val;
	t_dlist	*tmp;
	t_tok	*tok;
	int		rd_fl;

	rd_fl = 0;
	if (!tls || !(tok = tls->content) || end_tk == tls || (tok->type & endtk))
		return (1);
	if (tok->type & (TK_RD_A | TK_RD_R | TK_RD_RW | TK_RD_W))
	{
		tmp = arg_tok_skip(tls->next, TK_EMPTY);
		if (tmp && tmp->content && (((t_tok *)tmp->content)->type \
		& (TK_FILENAME | TK_PROF_IN | TK_PROF_OUT)))
			rd_fl = 1;
	}
	if (!(val = token_get_dvalue(tok, rd_fl)) || \
	dstr_insert_dstr(buff, val, buff->strlen) < 0)
		sys_fatal_memerr("PRS_TKS_TO_ILN");
	dstr_del(&val);
	return (0);
}

char		*str_from_n_tkls(t_dlist *toks, size_t tlen, \
t_dlist *end_tk, t_tk_type end_t_tk)
{
	DSTRING *buff;
	char	*ret;

	if (!(buff = dstr_new("")))
		sys_fatal_memerr("PRS_TKS_TO_ILN");
	while (tlen-- > 0 && !(str_from_tok_it(buff, toks, end_tk, end_t_tk)))
		toks = toks->next;
	if (tlen == 0)
		dstr_insert_str(buff, " (...)", buff->strlen);
	ret = buff->txt;
	buff->txt = 0;
	dstr_del(&buff);
	return (ret);
}
