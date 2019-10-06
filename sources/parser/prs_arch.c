/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 04:13:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/07 01:45:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"
#include "ft_io.h"
#include "sys_tools/sys_tools.h"

#include "stdio.h"

extern pid_t	hot_gid;

int				prs_execute_expr(ETAB **etab ,ENV *envs)
{
	ETAB		*etab_row;
	ETAB		*pipe_free;
	int			status;
	int			signal_v;
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
		if (etab_row->type == ET_PIPE && !(cpid = 0))
			ft_dlstunshift((t_dlist **)&pipe_free, (t_dlist *)etab_row);
		else
		{
			cpid < 0 ? prs_error_handler(-1 * cpid, &status, envs, etab_row->instruction) : 0;
			ft_dlst_delcut((t_dlist **)&etab_row, et_rm_ett);
		}
		if (cpid > 0)
		{
			// SETTING PGID FOR ALL GROUP PIDS AND TAKING THEM INTO P_TABLE
			sys_hot_charge(cpid, PS_M_FG, 0);
			setpgid(cpid, hot_gid);
		}
	}
	ft_dlst_delf((t_dlist **)&pipe_free, (size_t)-1, et_rm_ett);
	if (cpid > 0)
	{
		// if we have a forked process group, we give terminal control to it.
		tcsetpgrp(0, hot_gid);
	}
	signal_v = sys_wait_ptable(&status, cpid);
	// DBG_SYS_SNAP();
	sys_hot_off(2);
	prs_set_last_status(&status, envs);
	sys_kill_pipes();
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

/*
!! TEMPORARY FUNCTION
!! Soon will be changed!
*/
int				subsh_to_expr_maker(ETAB **etab)
{
	SUBSH		*subsh;
	EXPRESSION	*cmd;
	char		*tmp;

	subsh = (*etab)->instruction;
	tmp = subsh->commands;
	(*etab)->type = ET_EXPR;
	cmd = (EXPRESSION *)subsh;
	if (!(cmd->args = ft_memalloc(4 * sizeof(char *))))
		return (-1);
	cmd->args[0] = ft_strdup("bash"); // TODO: need to handle malloc fails
	cmd->args[1] = ft_strdup("-c");
	cmd->args[2] = tmp;
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
		if (etab_row->type == ET_SUBSH)
			status = subsh_to_expr_maker(etab);
		if (etab_row->type == ET_EXPR)
			status = prs_execute_expr(etab, envs);
	}
	return (status);
}

t_dlist			*sh_tparse(t_dlist *tks, ENV *envs, t_tk_type end_tk, int *status)
{
	ETAB		*etab;
	t_tok		*tok;

	etab = 0;
	while (tks && (tok = tks->content))
	{
		if (etab && (tok->type & (TK_SEPS1 | end_tk)))
			*status = prs_executor(&etab, envs); // TODO: ERROR CHECKING NEED
		if (tok->type & (TK_BREAK | TK_CONTIN))
			return (tks);
		if (tok->type & (end_tk | TK_EOF))
			break ;
		tks = tok->type == TK_EXPR ? prs_expr(&etab, tks, envs) : tks;
		tks = tok->type == TK_FUNCTION ? prs_func(tks, envs) : tks;
		tks = tok->type == TK_MATH ? prs_math(&etab, tks, envs) : tks;
		tks = tok->type == TK_PIPE ? prs_pipe(&etab, tks) : tks; 
		tks = tok->type == TK_IF ? prs_if(tks, envs, status) : tks;
		tks = tok->type == TK_WHILE ? prs_while(tks, envs, status) : tks;
		tks = tok->type == TK_FOR ? prs_for(tks, envs, status) : tks;
		tks = tok->type == TK_VAR ? prs_assigm(tks, envs, status) : tks;
		tks = tok->type == TK_AND ? prs_and(tks, envs, status) : tks;
		tks = tok->type == TK_OR ? prs_or(tks, envs, status) : tks;
		tks = tok->type == TK_SUBSH ? prs_subsh(&etab, tks, envs) : tks;
		tks = tok->type & (TK_EMPTY | TK_SEPS1 | (TK_FLOWS & ~(TK_IF | TK_WHILE))) ? tks->next : tks;
	}
	return (tks);
}
