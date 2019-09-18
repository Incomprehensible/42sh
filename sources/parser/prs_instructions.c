/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_instructions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:26:19 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/17 19:02:53 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_dlist			*prs_expr(ETAB **tab, t_dlist *tokens, ENV *envs)
{
	EXPRESSION	*expr;
	ETAB		*nrow;

	if (!(nrow = (ETAB *)ft_dlstnew_cc(0, 0)))
		return (0); // ERROR: prs_expr: Malloc failed.
	ft_dlstpush((t_dlist **)tab, (t_dlist *)nrow);
	nrow->type = ET_EXPR;
	if (!(nrow->instruction = ft_memalloc(sizeof(EXPRESSION))))
		return (0); // ERROR: prs_expr: Malloc failed.
	expr = (EXPRESSION *)nrow->instruction;
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		expr->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	if (!(expr->args = prs_args(tokens, envs)))
		return (0); // ERROR: prs_expr: Malloc failed.
	expr->redirections = prs_rdrs(&tokens);
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
		return (0); // ERROR: prs_math: Malloc failed.
	ft_dlstpush((t_dlist **)tab, (t_dlist *)nrow);
	nrow->type = ET_SUBSH;
	if (!(nrow->instruction = ft_memalloc(sizeof(MATH))))
		return (0); // ERROR: prs_math: Malloc failed.
	subsh = (SUBSH *)nrow->instruction;
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		subsh->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	subsh_tok = tokens->content;
	subsh->commands = (void *)ft_strdup(subsh_tok->value); 
	subsh->redirections = prs_rdrs(&tokens);
	return (tokens);
}

t_dlist			*prs_math(ETAB **tab, t_dlist *tokens, ENV *envs)
{
	MATH		*math;
	ETAB		*nrow;
	t_tok		*mtok;

	if (!(nrow = (ETAB *)ft_dlstnew_cc(0, 0)))
		return (0); // ERROR: prs_math: Malloc failed.
	ft_dlstpush((t_dlist **)tab, (t_dlist *)nrow);
	nrow->type = ET_MATH;
	if (!(nrow->instruction = ft_memalloc(sizeof(MATH))))
		return (0); // ERROR: prs_math: Malloc failed.
	math = (MATH *)nrow->instruction;
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		math->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	mtok = tokens->content;
	math->expr = ft_strdup(mtok->value); 
	math->redirections = prs_rdrs(&tokens);
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
		return (0); // ERROR: prs_pipe: Malloc failed.
	pipee = curt->instruction;
	if (curt->prev_e->type == ET_EXPR)
	{
		expr = curt->prev_e->instruction;
		expr->opipe_fds = pipee->pirw;
	}
	else if (curt->prev_e->type == ET_MATH)
	{
		math = curt->prev_e->instruction;
		math->opipe_fds = pipee->pirw;
	}
	return (tk->next);
}
