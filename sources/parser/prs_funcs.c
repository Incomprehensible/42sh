/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 17:59:40 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/17 16:27:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_errors.h"

static t_dlist	*prs_f_dup_tks(t_dlist *tks, t_dlist **fcode)
{
	t_tok		*tok;
	t_tok		*ntok;
	t_dlist		*instr;
	char		*val;

	while (tks && (tok = tks->content))
	{
		val = 0;
		if (!(ntok = ft_memalloc(sizeof(t_tok))))
			sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
		if (tok->value && !(val = ft_strdup(tok->value)))
			sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
		ntok->value = val;
		ntok->type = tok->type;
		if (!(instr = ft_dlstnew_cc(ntok, tks->size)))
			sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
		ft_dlstpush(fcode, instr);
		tks = tks->next;
		if (tok->type == TK_FEND)
			break ;
	}
	return (tks);
}

t_dlist			*prs_func(t_dlist *tks, ENV *envr)
{
	t_avln		*node;
	t_tok		*tok;
	t_func		*func;

	tks = tks->next;
	tok = tks->content;
	if (!(func = ft_memalloc(sizeof(FUNC))))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	node = ft_avl_node_cc(tok->value, func, sizeof(FUNC));
	tks = tks->next;
	if (!(tks = prs_f_dup_tks(tks, &func->func_code)))
		return (0);
	ft_avl_set(envr->funcs, node);
	return (tks);
}

t_dlist			*make_lambda_expr(char **args, \
ETAB **etab, ENV *env, t_dlist *tks)
{
	EXPRESSION	*expr;
	ETAB		*nrow;

	if (!(nrow = (ETAB *)ft_dlstnew_cc(0, 0)))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	nrow->type = ET_EXPR;
	if (!(nrow->instruction = ft_memalloc(sizeof(EXPRESSION))))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	expr = (EXPRESSION *)nrow->instruction;
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		expr->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	expr->args = args;
	expr->redirections = prs_rdrs(&tks, env);
	ft_dlstpush((t_dlist **)etab, (t_dlist *)nrow);
	return (tks);
}

t_dlist			*prs_lambda(t_dlist *tks, ENV *envr, ETAB **etab)
{
	t_avln		*node;
	t_func		*func;
	char		**args;

	tks = tks->next;
	if (!(func = ft_memalloc(sizeof(FUNC))))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	node = ft_avl_node_cc("Вы нашли фичу, поздравляем!", func, sizeof(FUNC));
	if (!(tks = prs_f_dup_tks(tks, &func->func_code)))
		return (0);
	ft_avl_set(envr->funcs, node);
	args = ft_strsplit(
	"Вы нашли фичу, поздравляем!\n"
	"Ваша внимательность достойна награды.\n"
	"Если вы смогли увидеть эти сообщения, вас ждет бесплатное пиво\n"
	"Обращайтесь к @hgranule",
	'\n');
	if (!args)
		sys_fatal_memerr("ARGUMNETS AT LAMDA EXPRESSION, MALLOC RETURNED 0");
	tks = make_lambda_expr(args, etab, envr, tks);
	return (tks);
}
