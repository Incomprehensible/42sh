/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 09:00:27 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/01 18:26:13 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_token_to_str.h"
#include "sys_tools/sys_errors.h"

#include "ft_mem.h"
#include "env.h"

#include <stdio.h>

extern pid_t	g_hsh;
extern char		*g_hbg;
extern int		g_prompt_fd;
extern int		g_jid;
extern int		g_intr;

int				sys_var_stdname_pr(ENV *env)
{
	t_dyn_string	*val;

	val = dstr_new(SHELL_NAME_STD);
	if ((env_set_variable(SH_VAR_SHNAME, val, env)) <= 0)
		sys_fatal_memerr("MALLOC CALL FAILED, VAR INIT");
	if (dstr_insert_str(val, "$ ", val->strlen) < 0)
		sys_fatal_memerr("MALLOC CALL FAILED, VAR INIT");
	if ((env_set_variable(SH_VAR_PROMPT, val, env)) <= 0)
		sys_fatal_memerr("MALLOC CALL FAILED, VAR INIT");
	dstr_del(&val);
	return (0);
}

int				sys_var_init(ENV *env)
{
	sys_var_stdname_pr(env);
	prs_set_pid(env);
	return (0);
}

int				sys_init(int sbh_on)
{
	extern t_pgrp	*g_ptab[SYS_PRGS_SIZE];
	extern char		g_pipes[SYS_PIPES_SIZE];

	sys_sig_init();
	g_hsh = 0;
	g_jid = 1;
	g_intr = 0;
	g_prompt_fd = dup2(2, PROMPT_OUT_FD);
	if (g_prompt_fd < 0)
		g_prompt_fd = STDERR_FILENO;
	ft_bzero(g_ptab, SYS_PRGS_SIZE * sizeof(t_pgrp *));
	ft_bzero(g_pipes, SYS_PIPES_SIZE * sizeof(char));
	return (sbh_on);
}

DSTRING			*sys_get_prompt_num(ENV *envr, char type)
{
	DSTRING		*prm;
	char		*sprompt;

	sprompt = 0;
	if (type == 0)
	{
		if (!(prm = env_get_variable(SH_VAR_PROMPT, envr)))
			sys_fatal_memerr("PROMT ALLOC FAILED");
		if (prm->strlen != 0)
			return (parse_promt(prm, envr));
		else
			dstr_del(&prm);
	}
	sprompt = type == 'a' ? SH_PROMPT_AP : sprompt;
	sprompt = type == 'q' ? SH_PROMPT_QT : sprompt;
	sprompt = type == 'h' ? SH_PROMPT_HD : sprompt;
	sprompt = type == 's' ? SH_PROMPT_SBH : sprompt;
	sprompt = type == 'l' ? SH_PROMPT_LM : sprompt;
	sprompt = type == 'p' ? SH_PROMPT_PIP : sprompt;
	sprompt = type == '&' ? SH_PROMPT_AND : sprompt;
	sprompt = type == '|' ? SH_PROMPT_OR : sprompt;
	sprompt = type == 'm' ? SH_PROMPT_MATH : sprompt;
	sprompt = sprompt == 0 ? SH_PROMPT : sprompt;
	if (!(prm = dstr_new(sprompt)))
		sys_fatal_memerr("PROMT ALLOC FAILED");
	return (prm);
}
