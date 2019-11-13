/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 09:00:27 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/13 22:36:19 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_sh_configs.h"
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

int				sys_var_init(ENV *env, char **argv, int argc)
{
	sys_var_stdname_pr(env);
	prs_set_pid(env);
	return (0);
}

void			sighand(int s)
{
	printf("SIGNAL input: %d (%d)\n", s, g_hsh);
	g_intr = s;
}

void			sys_sig_dfl(void)
{
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
}

void			sys_sig_init(void)
{
	signal(SIGINT, sighand);
	signal(SIGQUIT, sighand);
	signal(SIGTSTP, sighand);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
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
