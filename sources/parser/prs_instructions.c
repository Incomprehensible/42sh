/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_instructions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:26:19 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/29 01:26:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_errors.h"

t_dlist			*prs_expr(ETAB **tab, t_dlist *tokens, ENV *envs)
{
	EXPRESSION	*expr;
	ETAB		*nrow;
	t_dlist		*tks;

	if (!(nrow = (ETAB *)ft_dlstnew_cc(0, 0)))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	ft_dlstpush((t_dlist **)tab, (t_dlist *)nrow);
	nrow->type = ET_EXPR;
	if (!(nrow->instruction = ft_memalloc(sizeof(EXPRESSION))))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	expr = (EXPRESSION *)nrow->instruction;
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		expr->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	tks = tokens;
	if (!(expr->args = prs_args(tks, envs)))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	expr->redirections = prs_rdrs(&tokens, envs);
	return (tokens);
}

/*
!! TEMPORARY FUNCTION
!! Soon will be changed!
*/
t_dlist			*prs_subsh(ETAB **tab, t_dlist *tokens, ENV *envs)
{
	SUBSH		*subsh;
	ETAB		*nrow;
	t_tok		*subsh_tok;

	if (!(nrow = (ETAB *)ft_dlstnew_cc(0, 0)))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	ft_dlstpush((t_dlist **)tab, (t_dlist *)nrow);
	nrow->type = ET_SUBSH;
	if (!(nrow->instruction = ft_memalloc(sizeof(MATH))))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	subsh = (SUBSH *)nrow->instruction;
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		subsh->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	subsh_tok = tokens->content;
	if (!(subsh->commands = (void *)ft_strdup(subsh_tok->value)))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	subsh->redirections = prs_rdrs(&tokens, envs);
	return (tokens);
}

t_dlist			*prs_math(ETAB **tab, t_dlist *tokens, ENV *envs)
{
	MATH		*math;
	ETAB		*nrow;
	t_tok		*mtok;

	if (!(nrow = (ETAB *)ft_dlstnew_cc(0, 0)))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	ft_dlstpush((t_dlist **)tab, (t_dlist *)nrow);
	nrow->type = ET_MATH;
	if (!(nrow->instruction = ft_memalloc(sizeof(MATH))))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	math = (MATH *)nrow->instruction;
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		math->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	mtok = tokens->content;
	if (!(math->expr = ft_strdup(mtok->value)))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	math->redirections = prs_rdrs(&tokens, envs);
	return (tokens);
}

t_dlist			*prs_pipe(ETAB **tab, t_dlist *tk)
{
	ETAB		*curt;
	EXPRESSION	*expr;
	MATH		*math;
	PIPE		*pipee;

	curt = (ETAB *)ft_dlstnew_cc(0, 0);
	ft_dlstpush((t_dlist **)tab, (t_dlist *)curt);
	curt->type = ET_PIPE;
	if (!(curt->instruction = ft_memalloc(sizeof(PIPE))))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	pipee = curt->instruction;
	expr = curt->prev_e->instruction;
	expr->opipe_fds = pipee->pirw;
	return (tk->next);
}
