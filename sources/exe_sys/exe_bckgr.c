/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_bckgr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 13:46:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 15:15:00 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_token_to_str.h"
#include "executer.h"
#include "parser.h"
#include "sh_tokenizer.h"

#include "sys_tools/sys_dbg.h"

extern ENV	*g_env;

int		sbsh_is_fork_n_needp(t_dlist *tl)
{
	t_tok	*tok;

	while (tl)
	{
		tok = tl->content;
		if (tok->type & (TK_SEPS & ~TK_PIPE))
		{
			if (tok->type & (TK_EOF | TK_ARSHLOCH))
				return (1);
			else if (tok->type & (TK_AND | TK_OR))
				break ;
			tl = arg_tok_skip(tl->next, TK_EMPTY);
			if ((tok = tl->content)->type & (TK_EOF | TK_ARSHLOCH))
				return (1);
			else
				break ;
		}
		if (tok->type == TK_DEREF)
			tl = tl->next;
		if (!(tok->type & (TK_EXPR | TK_EMPTY | \
		0xf0000000000 | TK_VALUE | TK_NAME | 0x3f81 | TK_PIPE)))
			break ;
		tl = tl->next;
	}
	return (0);
}

int		exe_bkgr_alg(BCKGR *bkgr, ENV *envr, int *status)
{
	extern pid_t	g_hsh;
	t_pgrp			*pg;

	sys_init(1);
	g_hsh = getpid();
	pg = sys_prg_create(g_hsh, 0, 0, PS_M_FG);
	sh_tparse(bkgr->tls, envr, TK_ARSHLOCH | TK_EOF, status);
	sys_delete_prg(&pg);
	exit(*status);
}

int		exe_pipe_bckgr(BCKGR *bkgr, ENV *envr, int *status)
{
	g_hbg = "OK";
	sh_tparse(bkgr->tls, envr, TK_ARSHLOCH | TK_EOF, status);
	return (0);
}

int		exe_bkgr_expr(BCKGR *bkgr, ENV *envr, int *status)
{
	pid_t			cpid;
	extern pid_t	g_hsh;
	extern char		*g_hbg;
	SUBSH			f_subsh;

	*status = 0;
	ft_bzero(&f_subsh, sizeof(SUBSH));
	if (sbsh_is_fork_n_need(bkgr->tls))
		cpid = exe_one_command_lnch(&f_subsh, bkgr->tls, envr, status);
	else if (sbsh_is_fork_n_needp(bkgr->tls))
		return (exe_pipe_bckgr(bkgr, envr, status));
	else
	{
		cpid = fork();
		if (cpid < 0 && (*status = 2))
			return (-E_FRKFL);
		else if (cpid == 0)
			exe_bkgr_alg(bkgr, envr, status);
	}
	if (cpid < 0)
		return ((int)cpid);
	g_hbg = "OK";
	g_hsh = 0;
	return ((int)cpid);
}
