/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_instructions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:26:19 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/27 19:50:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_dlist			*prs_expr(ETAB **tab, t_dlist *tokens, ENV *envs)
{
	EXPRESSION	*expr;
	ETAB		*nrow;

	if (!(nrow = (ETAB *)ft_dlstnew_cc(0, 0)))
		return (0); //! MALLOC CALL FAILED. (SU.)
	ft_dlstpush((t_dlist **)tab, (t_dlist *)nrow);
	nrow->type = ET_EXPR;
	if (!(nrow->instruction = ft_memalloc(sizeof(EXPRESSION))))
		return (0); //! MALLOC CALL FAILED. (SU.)
	expr = (EXPRESSION *)nrow->instruction;
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		expr->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	if (!(expr->args = prs_args(tokens, envs)))
		return (0); //! MALLOC CALL FAILED. (NOT SAFE.)
	expr->redirections = prs_rdrs(&tokens);
	return (tokens);
}

t_dlist			*prs_pipe(ETAB **tab, t_dlist *tk)
{
	ETAB		*curt;
	EXPRESSION	*expr;
	PIPE		*pipee;

	curt = (ETAB *)ft_dlstnew_cc(0, 0);
	ft_dlstpush((t_dlist **)tab, (t_dlist *)curt);
	curt->type = ET_PIPE;
	if (!(curt->instruction = ft_memalloc(sizeof(PIPE))))
		return (0); //! MALLOC FAILED
	expr = curt->prev_e->instruction;
	pipee = curt->instruction;
	expr->opipe_fds = pipee->pirw;
	return (tk->next);
}
