/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 04:13:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/02 09:10:58 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "ft_io.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_token_to_str.h"

#include "stdio.h"

extern pid_t	hot_gid;
extern pid_t	hot_sbsh;
extern char		*hot_bkgr;

int				pex_p_table_pgid(pid_t cpid, char *i_line)
{
	int			mode;
	char		*str;

	mode = hot_bkgr ? PS_M_BG : PS_M_FG;
	str = i_line;
	if (hot_sbsh)
		hot_gid = hot_sbsh;
	sys_hot_charge(cpid, mode, str);
	setpgid(cpid, hot_gid);
	return (0);
}

int				prs_etab_handlers(ETAB **etab_row, ETAB **pipe_cache, int *status, ENV *envr)
{
	pid_t		cpid;

	cpid = 0;
	sys_sig_dfl();
	if ((*etab_row)->type == ET_EXPR)
		cpid = exe_execute_expr((*etab_row)->instruction, envr, status);
	// NEW SUBSHELL EXECUTING
	else if ((*etab_row)->type == ET_SUBSH)
		cpid = exe_subshell_expr((*etab_row)->instruction, envr, status);
	// NEW BACKGROUND EXECUTING
	else if ((*etab_row)->type == ET_BCKGR)
		cpid = exe_bkgr_expr((*etab_row)->instruction, envr, status);
	if ((*etab_row)->type == ET_PIPE && !(cpid = 0))
		ft_dlstunshift((t_dlist **)pipe_cache, (t_dlist *)(*etab_row));
	else
	{
		cpid < 0 ? prs_error_handler(-1 * cpid, (*etab_row)->type, envr, (*etab_row)->instruction) : 0;
		ft_dlst_delcut((t_dlist **)etab_row, et_rm_ett);
	}
	sys_sig_init();
	return (cpid);
}

int				prs_execute_expr(ETAB **etab ,ENV *envs)
{
	ETAB		*etab_row;
	ETAB		*pipe_free;
	int			status;
	pid_t		cpid;
	char		*i_line;

	etab_row = 0;
	pipe_free = 0;
	status = 127;
	while (*etab && ((*etab)->type == ET_EXPR || (*etab)->type == ET_PIPE \
	|| (*etab)->type == ET_SUBSH || (*etab)->type == ET_BCKGR))
	{
		i_line = sys_get_str_from_etab(etab);
		etab_row = (ETAB *)ft_dlstshift((t_dlist **)etab);
		cpid = prs_etab_handlers(&etab_row, &pipe_free, &status, envs);
		if (cpid > 0)
			pex_p_table_pgid(cpid, i_line);
		else if (i_line)
			free(i_line);
	}
	ft_dlst_delf((t_dlist **)&pipe_free, (size_t)-1, et_rm_ett);
	if (cpid > 0)
	{
		if (!hot_bkgr && !hot_sbsh)
			tcsetpgrp(0, hot_gid);
		else
			hot_bkgr = 0;
	}
	sys_wait_ptable(&status, cpid);
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
		if (etab_row->type == ET_EXPR || etab_row->type == ET_SUBSH || \
		etab_row->type == ET_BCKGR)
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
		if (g_intr)
		{
			g_intr = 0;
			ft_dlst_delf((t_dlist **)(&etab), 0, et_rm_ett);
			return (INTERRUPT_CALL);
		}
		if (etab && (tok->type & (TK_SEPS1 | end_tk | TK_ARSHLOCH)))
			*status = prs_executor(&etab, envs); // TODO: ERROR CHECKING NEED
		if (tok->type & (end_tk | TK_EOF | TK_BREAK | TK_CONTIN))
			return (tks);
		tks = tok->type & (TK_EXPR | TK_DEREF | TK_RDS1 | TK_FD) ? prs_expr(&etab, tks, envs) : tks;
		tks = tok->type == TK_FUNCTION ? prs_func(tks, envs) : tks;
		tks = tok->type == TK_BCKR_PS ? prs_bkgr(&etab, tks, envs) : tks;
		tks = tok->type == TK_MATH ? prs_math(&etab, tks, envs) : tks;
		tks = tok->type == TK_PIPE ? prs_pipe(&etab, tks) : tks; 
		tks = tok->type == TK_IF ? prs_if(tks, envs, status) : tks;
		tks = tok->type == TK_WHILE ? prs_while(tks, envs, status) : tks;
		tks = tok->type == TK_FOR ? prs_for(tks, envs, status) : tks;
		tks = tok->type == TK_VAR ? prs_assigm(tks, envs, status) : tks;
		tks = tok->type == TK_AND ? prs_and(tks, envs, status) : tks;
		tks = tok->type == TK_OR ? prs_or(tks, envs, status) : tks;
		tks = tok->type == TK_SUBSH ? prs_subsh(&etab, tks, envs) : tks;
		if (tks == INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		tks = tks && tks->next && tok->type & (TK_EMPTY | TK_SEPS1 | TK_ARSHLOCH | \
		(TK_FLOWS & ~(TK_IF | TK_WHILE))) & ~TK_EOF ? tks->next : tks;
	}
	return (tks);
}
