/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 04:13:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/01 17:42:19 by hgranule         ###   ########.fr       */
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
	cmd->args[0] = ft_strdup("((");
	cmd->args[2] = ft_strdup("))");
	cmd->args[1] = tmp;
	return (0);
}

int				prs_executor(ETAB **etab, ENV *envs) //TODO! ERROR CHECKING NEED
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
		if (tok->type == if_tk)
			tks = prs_skip_paired(tks->next, if_tk, fi_tk)->next;
		else if (tok->type == else_tk || tok->type == fi_tk)
			return(tks);
		else
			tks = tks->next;
	}
	return (tks);
}

t_dlist			*prs_if(t_dlist *tks, ENV *envs, int *status)
{
	t_tok			*tok;

	tks = sh_tparse(tks->next, envs, then_tk, status);
	if (*status == EXIT_SUCCESS)
	{
		tks = sh_tparse(tks, envs, else_tk, status);
		tok = tks->content;
		if (tok->type == break_tk)
			return (tks);
		tks = prs_skip_paired(tks, if_tk, fi_tk)->next;
	}
	else
	{
		tks = prs_skip_after_else(tks)->next;
		tks = sh_tparse(tks, envs, fi_tk, status);
		tok = tks->content;
		if (tok->type == break_tk)
			return (tks);
	}
	return (tks);
}

t_dlist			*prs_while(t_dlist *tks, ENV *envs, int *status)
{
	const t_dlist	*cond = tks->next;
	t_dlist			*end;
	t_tok			*tok;

	while ((end = sh_tparse((t_dlist *)cond, envs, do_tk, status)) && *status == EXIT_SUCCESS)
	{
		tks = sh_tparse(end, envs, done_tk, status);
		tok = tks->content;
		if (tok->type == break_tk)
			break ;
		if (tok->type == contin_tk)
			continue ;
	}
	return (prs_skip_paired(end, do_tk, done_tk)->next);
}

t_dlist			*sh_tparse(t_dlist *tks, ENV *envs, t_tk_type end_tk, int *status)
{
	ETAB		*etab;
	t_tok		*tok;

	etab = 0;
	while (tks && (tok = tks->content))
	{
		if (etab && (tok->type == sep_tk || tok->type == end_tk))
		{
			*status = prs_executor(&etab, envs); //TODO! ERROR CHEKING NEED
			ft_putstr("RETURNED STATUS: ");
			ft_putnbr(*status);
			ft_putstr("\n");
		}

		if (tok->type == break_tk || tok->type == contin_tk)
			return (tks);
		if (tok->type == fi_tk && end_tk == else_tk)
			break ;
		if (tok->type == end_tk || tok->type == eof_tk)
			break ;

		tks = tok->type == empty_tk || tok->type == sep_tk ? tks->next : tks;
		tks = tok->type == expr_tk ? prs_expr(&etab, tks, envs) : tks;
		tks = tok->type == math_tk ? prs_math(&etab, tks, envs) : tks;
		tks = tok->type == pipe_tk ? prs_pipe(&etab, tks) : tks; 
		tks = tok->type == if_tk ? prs_if(tks, envs, status) : tks;
		tks = tok->type == while_tk ? prs_while(tks, envs, status) : tks;
	}
	tks = tks && (tok = tks->content) && tok->type == end_tk ? tks->next : tks;
	return (tks); //! MALLOC CALL FAILED.
}
