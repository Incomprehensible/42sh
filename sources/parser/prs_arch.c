/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 04:13:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/22 14:24:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executer.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "ft_io.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_token_to_str.h"

extern pid_t	g_hgid;
extern pid_t	g_hsh;
extern char		*g_hbg;
ENV				*g_env;

int				pex_g_ptab_pgid(pid_t cpid, char *i_line)
{
	int			mode;
	char		*str;

	mode = g_hbg ? PS_M_BG : PS_M_FG;
	str = i_line;
	if (g_hsh)
		g_hgid = g_hsh;
	sys_hot_charge(cpid, mode, str);
	setpgid(cpid, g_hgid);
	if (cpid > 0)
	{
		if (!g_hbg && !g_hsh)
			tcsetpgrp(0, g_hgid);
		else
			g_hbg = 0;
	}
	return (0);
}

int				prs_etab_handlers(ETAB **etab_row, \
ETAB **pipe_cache, int *status, ENV *envr)
{
	pid_t		cpid;
	int			a;

	cpid = 0;
	a = 0;
	if ((*etab_row)->type == ET_EXPR)
		cpid = exe_execute_expr((*etab_row)->instruction, envr, status);
	else if ((*etab_row)->type == ET_SUBSH)
		cpid = exe_subshell_expr((*etab_row)->instruction, envr, status);
	else if ((*etab_row)->type == ET_BCKGR)
		cpid = exe_bkgr_expr((*etab_row)->instruction, envr, status);
	if ((*etab_row)->type == ET_PIPE && !(cpid = 0))
		ft_dlstunshift((t_dlist **)pipe_cache, (t_dlist *)(*etab_row));
	else
	{
		cpid < 0 ? prs_error_handler(-1 * cpid, \
		(*etab_row)->type, envr, (*etab_row)->instruction) : 0;
		ft_dlst_delcut((t_dlist **)etab_row, et_rm_ett);
	}
	return (cpid);
}

int				prs_post_execute_xp(int *status, pid_t cpid, ENV *env)
{
	sys_wait_ptable(status, cpid);
	sys_hot_off(2);
	prs_set_last_status(status, env);
	sys_kill_pipes();
	return (*status);
}

t_dlist			*sh_tparse_tks(t_dlist *tks, int *status, \
ETAB **etab, t_tk_type type)
{
	tks = type & (TK_EXPR | TK_DEREF | TK_RDS1 | TK_FD) ? \
	prs_expr(etab, tks, g_env) : tks;
	tks = type == TK_FUNCTION ? prs_func(tks, g_env) : tks;
	tks = type == TK_BCKR_PS ? prs_bkgr(etab, tks, g_env) : tks;
	tks = type == TK_MATH ? prs_math(etab, tks, g_env) : tks;
	tks = type == TK_PIPE ? prs_pipe(etab, tks) : tks;
	tks = type == TK_IF ? prs_if(tks, g_env, status) : tks;
	tks = type == TK_WHILE ? prs_while(tks, g_env, status) : tks;
	tks = type == TK_FOR ? prs_for(tks, g_env, status) : tks;
	tks = type == TK_VAR ? prs_assigm(tks, g_env, status) : tks;
	tks = type == TK_AND ? prs_and(tks, g_env, status) : tks;
	tks = type == TK_OR ? prs_or(tks, g_env, status) : tks;
	tks = type == TK_SUBSH ? prs_subsh(etab, tks, g_env) : tks;
	return (tks);
}

t_dlist			*sh_tparse(t_dlist *tks, ENV *envs, \
t_tk_type end_tk, int *status)
{
	ETAB		*etab;
	t_tok		*tok;

	etab = 0;
	g_env = envs;
	while (tks && (tok = tks->content))
	{
		if (g_intr)
		{
			g_intr = 0;
			ft_dlst_delf((t_dlist **)(&etab), 0, et_rm_ett);
			return (INTERRUPT_CALL);
		}
		if (etab && (tok->type & (TK_SEPS1 | end_tk | TK_ARSHLOCH)))
			*status = prs_executor(&etab, envs);
		if (tok->type & (end_tk | TK_EOF | TK_BREAK | TK_CONTIN))
			return (tks);
		tks = sh_tparse_tks(tks, status, &etab, tok->type);
		if (tks == INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		tks = tks && tks->next && tok->type & (TK_ARSHLOCH | TK_SEPS1 | \
		TK_EMPTY | (TK_FLOWS & ~(TK_IF | TK_WHILE))) & ~TK_EOF ? \
		tks->next : tks;
	}
	return (tks);
}
