/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 04:13:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/11 08:53:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"
#include "ft_io.h"

int				prs_execute_expr(ETAB **etab ,ENV *envs)
{
	ETAB		*etab_row;
	ETAB		*pipe_free;
	int			status;
	pid_t		cpid;

	etab_row = 0;
	pipe_free = 0;
	cpid = 0;
	status = 127;
	while (*etab && ((*etab)->type == ET_EXPR || (*etab)->type == ET_PIPE))
	{
		etab_row = (ETAB *)ft_dlstshift((t_dlist **)etab);
		if (etab_row->type == ET_EXPR)
			cpid = exe_execute_expr(etab_row->instruction, envs, &status);
		if (etab_row->type == ET_PIPE)
			ft_dlstunshift((t_dlist **)&pipe_free, (t_dlist *)etab_row);
		else
			ft_dlst_delcut((t_dlist **)&etab_row, et_rm_ett);
	}
	ft_dlst_delf((t_dlist **)&pipe_free, (size_t)-1, et_rm_ett);
	if (cpid > 0)
		status = exe_wait_cps(cpid);
	return (status);
}

int				math_to_expr_maker(ETAB **etab)
{
	MATH		*math;
	EXPRESSION	*cmd;
	char		*tmp;

	math = (*etab)->instruction;
	tmp = math->expr;
	(*etab)->type = ET_EXPR;
	cmd = (EXPRESSION *)math;
	if (!(cmd->args = ft_memalloc(4 * sizeof(char *))))
		return (-1);
	cmd->args[0] = ft_strdup("(("); // TODO: need to handle malloc fails
	cmd->args[2] = ft_strdup("))");
	cmd->args[1] = tmp;
	return (0);
}

int				prs_executor(ETAB **etab, ENV *envs) // TODO: ERROR CHECKING NEED
{
	ETAB		*etab_row;
	ETAB		*trash;
	int			status;
	pid_t		cpid;

	while ((etab_row = *etab) != 0)
	{
		if (etab_row->type == ET_MATH)
			status = math_to_expr_maker(etab);
		if (etab_row->type == ET_EXPR)
			status = prs_execute_expr(etab, envs);
	}
	return (status);
}

t_dlist			*prs_skip_paired(t_dlist *tks, t_tk_type op, t_tk_type cls)
{
	int			cntr;
	t_tok		*tok;

	cntr = -1;
	while (cntr && tks && (tok = tks->content))
	{
		if (tok->type == op)
			--cntr;
		if (tok->type == cls)
			++cntr;
		if (!cntr)
			break ;
		tks = tks->next;
	}
	return (tks);
}

t_dlist			*prs_skip_after_else(t_dlist *tks)
{
	t_tok		*tok;

	while (tks && (tok = tks->content))
	{
		if (tok->type == TK_IF)
			tks = prs_skip_paired(tks->next, TK_IF, TK_FI)->next;
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
		tks = sh_tparse(tks, envs, TK_ELSE | TK_FI, status);
		tok = tks->content;
		if (tok->type == TK_BREAK)
			return (tks);
		tks = prs_skip_paired(tks, TK_IF, TK_FI)->next;
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
		tks = sh_tparse(end, envs, TK_DONE, status);
		tok = tks->content;
		if (tok->type == TK_BREAK)
			break ;
		if (tok->type == TK_CONTIN)
			continue ;
	}
	return (prs_skip_paired(end, TK_DO, TK_DONE)->next);
}

t_dlist			*sh_tparse(t_dlist *tks, ENV *envs, t_tk_type end_tk, int *status)
{
	ETAB		*etab;
	t_tok		*tok;

	etab = 0;
	while (tks && (tok = tks->content))
	{
		if (etab && (tok->type & (TK_SEP | end_tk)))
		{
			*status = prs_executor(&etab, envs); // TODO: ERROR CHECKING NEED
			//ft_putstr("RETURNED STATUS: ");
			//ft_putnbr(*status);
			//ft_putstr("\n");
		}

		if (tok->type & (TK_BREAK | TK_CONTIN))
			return (tks);
		if (tok->type & (end_tk | TK_EOF))
			break ;
		tks = tok->type & (TK_EMPTY | TK_SEP) ? tks->next : tks;
		tks = tok->type == TK_EXPR ? prs_expr(&etab, tks, envs) : tks;
		tks = tok->type == TK_MATH ? prs_math(&etab, tks, envs) : tks;
		tks = tok->type == TK_PIPE ? prs_pipe(&etab, tks) : tks; 
		tks = tok->type == TK_IF ? prs_if(tks, envs, status) : tks;
		tks = tok->type == TK_WHILE ? prs_while(tks, envs, status) : tks;
	}
	tks = tks && (tok = tks->content) && tok->type == end_tk ? tks->next : tks;
	return (tks);
}
