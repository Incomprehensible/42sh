/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 04:13:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/17 18:18:05 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

static int		prs_newdlst_rdr_init(t_dlist *token, t_dlist **res,\
									t_redir **redir)
{
	t_tok		*left;
	t_tok		*right;
	t_rdtype	type;

	type = (t_rdtype)token->content;
	left = token->prev ? (t_tok *)token->prev->content : 0;
	right = token->next ? (t_tok *)token->next->content : 0;
	if (!right || !right->value)
		return (0);
	if ((!left || !right || ((!left->value) || (!right->value))) && type == rw_rdr)
		return (0);
	if (!((*res) = (t_dlist *)malloc(sizeof(t_dlist))))
		return (0);
	if (!((*redir) = (t_redir *)malloc(sizeof(t_redir))))
	{
		//!!!!!!!!!!CLEAR res
		return (0);
	}
	return (1);
}

t_rdtype		prs_rdr_type(t_tok *tok)
{
	if (tok->type == rda_tk)
		return (a_rdr);
	if (tok->type == rdr_tk)
		return (r_rdr);
	if (tok->type == rdw_tk)
		return (w_rdr);
	return (rw_rdr);
}

t_dlist			*prs_newdlst_rdr(t_dlist *token)
{
	t_dlist		*res;
	t_tok		*left;
	t_tok		*right;
	t_redir		*redir;
	t_rdtype	type;

	type = prs_rdr_type((t_tok *)token->content);
	left = token->prev ? (t_tok *)token->prev->content : 0;
	right = token->next ? (t_tok *)token->next->content : 0;
	if (prs_newdlst_rdr_init(token, &res, &redir) == 0)
		return (0);
	if ((!left || left->type != fd_tk) && (type == w_rdr || type == a_rdr))
		redir->fdl = 1;
	else if ((!left || left->type != fd_tk) && type == r_rdr)
		redir->fdl = 0;
	if (left->type == fd_tk)
		redir->fdl = ft_atoi(left->value);
	if (right && right->type == filename_tk)
		redir->file = (char *)ft_strdup(right->value);
	else if (right)
		redir->fdr = ft_atoi(right->value);
	else
		; // PARSE ERROR
	redir->type = type;
	res->content = (t_redir *)redir;
	return (res);
}

static char		**prs_args_cr(t_dlist *tk)
{
	char		**args;
	t_dlist		*tki;
	t_tok		*tokn;
	size_t		args_len;
	size_t		i;

	// ПОДСЧЕТ КОЛИЧЕСТВА АРГВ
	args_len = 0;
	tki = tk;
	while (tki && (tokn = tki->content)->type != sep_tk \
	&& tokn->type != pipe_tk && tokn->type != eof_tk && ++args_len)
		tki = tki->next;
	// Выделение памяти на АРГВ
	if (!(args = ft_memalloc(sizeof(char *) * (args_len + 1))))
		; // TERMINATE
	// ЗАПОЛНЕНИЕ АРГВ
	tki = tk;
	i = -1;
	while (tki && (tokn = tki->content)->type != sep_tk \
	&& tokn->type != pipe_tk && tokn->type != eof_tk)
	{
		if (tokn->type == cmd_tk)
			args[++i] = ft_strdup(tokn->value);
		tki = tki->next;
	}
	return (args);
}

static int		prs_parse_rdr(EXPRESSION *expr, t_dlist **tk)
{
	t_dlist		*tki;
	t_dlist		*rdlist;
	t_dlist		*nlst;
	t_tok		*tokn;
	REDIRECT	rdrd;

	tki = *tk;
	rdlist = 0;
	while (tki && (tokn = tki->content)->type != sep_tk \
	&& tokn->type != pipe_tk && tokn->type != eof_tk)
	{
		if (tokn->type >= rdr_tk && tokn->type <= rdrw_tk)
		{
			nlst = prs_newdlst_rdr(tki); // IF 0, PARSE ERROR
			ft_dlstpush(&rdlist, nlst);
		}
		tki = tki->next;
	}
	expr->redirections = rdlist;
	*tk = tki;
	return (0);
}

static t_dlist	*prs_parse_tkcmd(EXTAB **tab, t_dlist *tk)
{
	EXTAB		*curt;
	EXPRESSION	*expr;
	PIPE		*pipee;
	t_tok		*tokn;
	t_dlist		*tki;

	// ВЫДЕЛЕНИЕ ПАМЯТИ НА ТАБЛИЦУ
	curt = (t_extab *)ft_dlstnew_cc(0, 0);
	ft_dlstpush((t_dlist **)tab, (t_dlist *)curt);
	curt->type = ETT_EXPR;
	if (!(curt->ex = ft_memalloc(sizeof(EXPRESSION))))
		; // TERMINATE
	expr = curt->ex;
	// Если предыдущая запись таблицы пайп, то подключаем его
	if (curt->prev_e && curt->prev_e->type == ETT_PIPE)
		expr->ipipe_fds = ((PIPE *)curt->prev_e->ex)->pirw;
	
	expr->args = prs_args_cr(tk); // ПОЛУЧАЕМ АРГВ
	prs_parse_rdr(expr, &tk);
	return (tk);
}

static t_dlist	*prs_parse_tkpipe(EXTAB **tab, t_dlist *tk)
{
	EXTAB		*curt;
	EXPRESSION	*expr;
	PIPE		*pipee;

	// ВЫДЕЛЕНИЕ ПАМЯТИ НА ТАБЛИЦУ
	curt = (t_extab *)ft_dlstnew_cc(0, 0);
	ft_dlstpush((t_dlist **)tab, (t_dlist *)curt);
	curt->type = ETT_PIPE;
	if (!(curt->ex = ft_memalloc(sizeof(PIPE))))
		; // TERMINATE
	// ЕСЛИ ПЕРДЫДУЩАЯ ЗАПИСЬ КОМАНДА, ТО ЕЁ ВЫВОД В ПАЙП
	expr = curt->prev_e->ex;
	pipee = curt->ex;
	expr->opipe_fds = pipee->pirw;

	return (tk->next);
}

EXTAB			*parser_get_from_tklist(t_dlist *tokens)
{
	EXTAB		*tab;
	EXTAB		*it;
	t_tok		*tkcnt;

	tab = 0;
	it = 0;
	while (tokens)
	{
		tkcnt = tokens->content;
		if (tkcnt->type == cmd_tk)
			tokens = prs_parse_tkcmd(&tab, tokens); // must return a new token pointer;
		if (tkcnt->type == pipe_tk)
			tokens = prs_parse_tkpipe(&tab, tokens);
		if (tkcnt->type == eof_tk)
			break ;
	}
	return (tab);
}
