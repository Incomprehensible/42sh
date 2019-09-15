/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_if_while.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:03:53 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/15 18:04:12 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_dlist	*prs_skip_after_else(t_dlist *tks)
{
	t_tok		*tok;

	while (tks && (tok = tks->content))
	{
		if (tok->type == TK_IF)
			tks = prs_skip_paired_trg(tks->next, TK_IF, TK_FI, -1)->next;
		else if (tok->type == TK_ELSE || tok->type == TK_FI)
			return(tks);
		else
			tks = tks->next;
	}
	return (tks);
}

t_dlist			*prs_if(t_dlist *tks, ENV *envs, int *status)
{
	t_tok			*tok;

	tks = sh_tparse(tks->next, envs, TK_THEN, status);
	if (*status == EXIT_SUCCESS)
	{
		tks = sh_tparse(tks->next, envs, TK_ELSE | TK_FI, status);
		tok = tks->content;
		if (tok->type == TK_BREAK)
			return (tks);
		tks = prs_skip_paired_trg(tks, TK_IF, TK_FI, -1);
	}
	else
	{
		tks = prs_skip_after_else(tks);
		if ((tok = tks->content)->type == TK_FI)
			return (tks->next);
		tks = sh_tparse(tks->next, envs, TK_FI, status);
		if ((tok = tks->content)->type == TK_BREAK)
			return (tks);
	}
	return (tks);
}

t_dlist			*prs_while(t_dlist *tks, ENV *envs, int *status)
{
	const t_dlist	*cond = tks->next;
	t_dlist			*end;
	t_tok			*tok;

	while ((end = sh_tparse((t_dlist *)cond, envs, TK_DO, status)) && *status == EXIT_SUCCESS)
	{
		tks = sh_tparse(end->next, envs, TK_DONE, status);
		tok = tks->content;
		if (tok->type == TK_BREAK)
			break ;
		if (tok->type == TK_CONTIN)
			continue ;
	}
	return (prs_skip_paired(end, TK_DO, TK_DONE)->next);
}
