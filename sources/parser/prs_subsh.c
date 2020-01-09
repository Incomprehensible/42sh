/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_subsh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 11:03:04 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/27 16:39:57 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executer.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"
#include "sh_tokenizer.h"
#include "ft_lbuffer.h"

#include "stdio.h"

extern pid_t	g_hsh;

int				io_subsh_ex(char *code, ENV *envr)
{
	t_pgrp		*pg;
	t_dlist		*toks[2];
	int			status;

	sys_init(1);
	g_hsh = getppid();
	ft_bzero(toks, sizeof(t_dlist *) * 2);
	if (sh_tokenizer(code, toks) <= 0)
	{
		sys_error_message("smt", 0);
		exit(255);
	}
	pg = sys_prg_create(g_hsh, 0, code, PS_M_FG);
	sh_tparse(toks[0], envr, TK_EOF, &status);
	sys_delete_prg(&pg);
	exit(status);
}

char			*get_deref_subsh(char *code, ENV *envr)
{
	int			pips[2];
	SUBSH		sbh;
	int			status;
	int			pid;
	char		*res;

	res = NULL;
	ft_bzero(&sbh, sizeof(SUBSH));
	sbh.commands = code;
	sbh.opipe_fds = pips;
	status = 127;
	if ((pid = exe_subshell_expr(&sbh, envr, &status)) > 0)
	{
		close(pips[1]);
		res = ft_lb_flush(ft_lb_readbytes(pips[0], 0));
		waitpid(pid, 0, 0);
		close(pips[0]);
	}
	return (res);
}

char			*ps_sbst_mods(int *pips, int mode)
{
	if (mode == 1)
	{
		close(pips[1]);
		dup2(pips[0], 0);
		close(pips[0]);
	}
	else if (mode == 2)
	{
		close(pips[0]);
		dup2(pips[1], 1);
		close(pips[1]);
	}
	else if (mode == 3)
	{
		sys_destroy_pipe(pips[0]);
		return (ft_itoa(pips[1]));
	}
	else if (mode == 4)
	{
		sys_destroy_pipe(pips[1]);
		return (ft_itoa(pips[0]));
	}
	return (0);
}

char			*prc_substitute(char *code, ENV *envr, int is_in)
{
	int				pips[2];
	pid_t			pid;
	t_dyn_string	*dstr;
	char			*tmp;

	sys_create_pipe(pips);
	if ((pid = fork()) == 0)
	{
		if (is_in)
			ps_sbst_mods(pips, 1);
		else
			ps_sbst_mods(pips, 2);
		io_subsh_ex(code, envr);
	}
	dstr = dstr_new("/dev/fd/");
	tmp = is_in ? ps_sbst_mods(pips, 3) : ps_sbst_mods(pips, 4);
	if (!tmp || !dstr)
		sys_fatal_memerr("PRC_SBST FAIL");
	dstr_insert_str(dstr, tmp, MAX_LL);
	free(tmp);
	if (!(tmp = ft_strdup(dstr->txt)) || !dstr)
		sys_fatal_memerr("PRC_SBST FAIL");
	dstr_del(&dstr);
	return (tmp);
}
