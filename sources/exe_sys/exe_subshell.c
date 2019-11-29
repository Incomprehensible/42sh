/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_subshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 11:41:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/29 22:10:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "executer.h"
#include "parser.h"
#include "sh_tokenizer.h"

int			sbsh_is_fork_n_need(t_dlist *tl, t_dlist *rdrs)
{
	t_tok	*tok;

	while (tl)
	{
		if ((tok = tl->content) && (tok->type & TK_SEPS))
		{
			if (tok->type & (TK_EOF | TK_ARSHLOCH))
				return (1);
			else if (tok->type & (TK_AND | TK_OR | TK_PIPE))
				break ;
			tl = arg_tok_skip(tl->next, TK_EMPTY);
			if ((tok = tl->content)->type & (TK_EOF | TK_ARSHLOCH))
				return (1);
			return (0);
		}
		if (tok && (tok->type & TK_RDS && rdrs))
			break ;
		if (tok && (tok->type == TK_DEREF))
			tl = tl->next;
		if (tok && !(tok->type & (TK_EXPR | TK_EMPTY | \
		0xf0000000000 | TK_VALUE | TK_NAME | 0x3f81)))
			break ;
		tl = tl->next;
	}
	return (0);
}

/*
** SUB-SHELL executing
** 1) Tokenizing string
** 2) If syntax error status set too 255 and leave
** 3) fork and save child's pid
**		0pf) pipes inits, rdrs inits
** 		1f) set g_hsh to getpid()
**		2f) shtparse tokens
**		3f) exit with code at *status
** 4) pipes init
** 5) return pid
*/

int			exe_sbsh_pipes(SUBSH *sb)
{
	if (sb->ipipe_fds && (dup2(sb->ipipe_fds[0], 0) >= 0))
	{
		close(sb->ipipe_fds[0]);
		close(sb->ipipe_fds[1]);
	}
	if (sb->opipe_fds && (dup2(sb->opipe_fds[1], 1) >= 0))
	{
		close(sb->opipe_fds[0]);
		close(sb->opipe_fds[1]);
	}
	return (0);
}

int			exe_subshell_alg(t_dlist *toks, SUBSH *sb, ENV *envr, int *status)
{
	t_dlist			*redirs;
	extern pid_t	g_hsh;
	t_pgrp			*pg;
	int				err;

	err = 0;
	redirs = sb->redirections;
	exe_sbsh_pipes(sb);
	err = exe_redir_list(redirs, envr);
	if (err)
		exit(2);
	sys_init(1);
	sys_sig_dfl();
	g_hsh = getpid();
	pg = sys_prg_create(g_hsh, 0, 0, PS_M_FG);
	sh_tparse(toks, envr, TK_EOF, status);
	exit(*status);
}

int			exe_one_command_lnch(SUBSH *subsh, t_dlist *tl, ENV *envr, int *st)
{
	t_extab		*etab;
	EXPRESSION	*xp;
	pid_t		cpid;

	etab = 0;
	prs_expr(&etab, tl, envr);
	close(g_prompt_fd);
	xp = (EXPRESSION *)etab->instruction;
	xp->ipipe_fds = subsh->ipipe_fds;
	xp->opipe_fds = subsh->opipe_fds;
	if (xp->ipipe_fds || xp->opipe_fds || \
	!(ft_avl_search(envr->builtns, xp->args[0]) || \
	ft_avl_search(envr->funcs, xp->args[0])))
	{
		cpid = exe_execute_expr(xp, envr, st);
		ft_dlst_delf((t_dlist **)&etab, 0, et_rm_ett);
		return (cpid);
	}
	else if ((cpid = fork()) < 0)
		return (-E_FRKFL);
	else if (cpid == 0 && (exe_execute_expr(xp, envr, st) || 1))
		exit(*st);
	return (cpid);
}

int			exe_subshell_expr(SUBSH *subsh, ENV *envr, int *status)
{
	t_dlist			*toks[2];
	pid_t			cpid;
	extern pid_t	g_hsh;

	ft_bzero(toks, sizeof(t_dlist *) * 2);
	if (sh_tokenizer(subsh->commands, toks) <= 0 && (*status = 255))
		return (subsh_tok_err("Subshell: syntax error!", 0, toks, envr));
	if (sbsh_is_fork_n_need(toks[0], subsh->redirections))
		cpid = exe_one_command_lnch(subsh, toks[0], envr, status);
	else
	{
		subsh->opipe_fds ? pipe(subsh->opipe_fds) : 0;
		if ((exe_heredoc_check(subsh->redirections, envr)))
			return (0);
		if ((cpid = fork()) < 0)
			return (-E_FRKFL);
		else if (cpid == 0)
			exe_subshell_alg(toks[0], subsh, envr, status);
		subsh->ipipe_fds ? close(subsh->ipipe_fds[0]) : 0;
		subsh->ipipe_fds ? close(subsh->ipipe_fds[1]) : 0;
	}
	if (!(g_hsh = 0) && cpid < 0)
		return ((int)cpid);
	ft_dlst_delf(toks, 0, free_token);
	return ((int)cpid);
}
