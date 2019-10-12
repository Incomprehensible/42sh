/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 09:00:27 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/12 06:01:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_errors.h"

#include "ft_mem.h"
#include "env.h"

#include <stdio.h>

int			sys_var_stdname_pr(ENV *env)
{
	DSTRING		*val;

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

int			sys_var_init(ENV *env, char **argv, int argc)
{
	sys_var_stdname_pr(env);
	prs_set_pid(env);
	return (0);
}

void		sighand(int s)
{
	printf("SIGNAL input: %d\n", s);
}

void		sys_sig_init(void)
{
	signal(SIGINT, sighand);
	signal(SIGQUIT, sighand);
	signal(SIGTSTP, sighand);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
}

int			sys_init(void)
{
	extern t_pgrp	*p_table[SYS_PRGS_SIZE];
	extern char		sys_pipes[SYS_PIPES_SIZE];
	extern pid_t	hot_sbsh;
	
	// Здесь будет идти установка стандартных обработчиков сигналов!
	sys_sig_init();
	hot_sbsh = 0;
	ft_bzero(p_table, SYS_PRGS_SIZE * sizeof(t_pgrp *));
	ft_bzero(sys_pipes, SYS_PIPES_SIZE * sizeof(char));
	return (0);
}
