/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_dbg_bltn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 19:10:28 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/15 19:31:29 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_dbg.h"
#include "sh_tokenizer.h"
#include "fcntl.h"
#include "libft.h"
#include "stdio.h"

int				bltn_dbg_snap(char **args, ENV *envr)
{
	if (args[1])
	{
		if (ft_strequ(args[1], "ps"))
			DBG_SYS_SNAP();
		if (ft_strequ(args[1], "gl"))
			DBG_SYS_GLB();
		if (ft_strequ(args[1], "tk-on"))
			dbg_tok_pr_flag = 1;
		if (ft_strequ(args[1], "tk-off"))
			dbg_tok_pr_flag = 0;
		if (ft_strequ(args[1], "gc"))
			DBG_SYS_CONFS();
		if (ft_strequ(args[1], "sleep"))
		{
			if (args[2])
				sleep((unsigned int)ft_atoi(args[2]));
			else
				sleep(10);
		}
	}
	return (0);
}

void			DBG_SYS_CONFS(void)
{
	char		cwd_buff[2048];
	
	getcwd(cwd_buff, 2048);
	dprintf(2, 
	"\n\033[31;1mDEBG_INFO_CONFIGS:\033[0m\n"
	"    FD_TABLE_SIZE ... %d\n"
	"    PID ............. %d\n"
	"    PPID ............ %d\n"
	"    PGRP ............ %d\n"
	"    CWD ............. %s\n"
	, getdtablesize(), getpid(), getppid(), getpgrp(), cwd_buff
	);
}

void			DBG_PRINT_TOKENS(t_dlist *toklst)
{
	t_tok		*token;
	char		*msg;
	char		*value;

	msg = 0;
	while (toklst)
	{
		token = toklst->content;
		msg = token->type == TK_EMPTY ? "-" : msg;
		msg = token->type == TK_EXPR ? "XP" : msg;
		msg = token->type == TK_SEP ? "SP" : msg;
		msg = token->type == TK_OR ? "OR" : msg;
		msg = token->type == TK_AND ? "AND" : msg;
		msg = token->type == TK_BCKR_PS ? "BGR" : msg;
		msg = token->type == TK_RD_W ? "rW" : msg;
		msg = token->type == TK_RD_R ? "rR" : msg;
		msg = token->type == TK_RD_A ? "rA" : msg;
		msg = token->type == TK_RD_RW ? "rRW" : msg;
		msg = token->type == TK_HERED ? "HRD" : msg;
		msg = token->type == TK_WORD ? "WRD" : msg;
		msg = token->type == TK_FD ? "FD" : msg;
		msg = token->type == TK_FILENAME ? "FLN" : msg;
		msg = token->type == TK_PIPE ? "PIPE" : msg;
		msg = token->type == TK_ASSIGM ? "ASG" : msg;
		msg = token->type == TK_NAME ? "NAME" : msg;
		msg = token->type == TK_VALUE ? "VAL" : msg;
		msg = token->type == TK_MATH ? "MATH" : msg;
		msg = token->type == TK_SUBSH ? "SBSH" : msg;
		msg = token->type == TK_DEREF ? "DRF" : msg;
		msg = token->type == TK_IF ? "IF" : msg;
		msg = token->type == TK_THEN ? "THEN" : msg;
		msg = token->type == TK_ELSE ? "ELSE" : msg;
		msg = token->type == TK_FI ? "FI" : msg;
		msg = token->type == TK_WHILE ? "WHL" : msg;
		msg = token->type == TK_DO ? "DO" : msg;
		msg = token->type == TK_DONE ? "DONE" : msg;
		msg = token->type == TK_FOR ? "FOR" : msg;
		msg = token->type == TK_IN ? "IN" : msg;
		msg = token->type == TK_BREAK ? "BRK" : msg;
		msg = token->type == TK_CONTIN ? "CNT" : msg;
		msg = token->type == TK_EXEC ? "eXC" : msg;
		msg = token->type == TK_FUNCTION ? "FUNC" : msg;
		msg = token->type == TK_LAMBDA ? "LMD" : msg;
		msg = token->type == TK_RETURN ? "RT" : msg;
		msg = token->type == TK_EOF ? "EOF" : msg;
		msg = token->type == TK_FEND ? "FND" : msg;
		msg = token->type == TK_VAR ? "VAR" : msg;
		msg = token->type == TK_UNTIL ? "UTL" : msg;
		msg = token->type == TK_PROC_IN ? "PSIN" : msg;
		msg = token->type == TK_PROC_OUT ? "PSOU" : msg;
		msg = token->type == TK_PROF_IN ? "PFIN" : msg;
		msg = token->type == TK_PROF_OUT ? "PFOU" : msg;
		msg = token->type == TK_ARSHLOCH ? "AHLH" : msg;
		value = token->value ? token->value : "<->";
		dprintf(2, "%5s: %s\n", msg, value);
		toklst = toklst->next;
	}
}

// DEBUG
int			DBG_SYS_SNAP(void)
{
	ssize_t		i;
	t_pgrp		*pg;
	t_ps_d		*psd;
	char		*states[] = {"NULL", "DONE", "SIGNALED", "STOPPED", "RUNNING"};
	char		*mode[] = {"NULL", "FG", "BG"};
	t_dlist		*pids;

	i = -1;
	while (++i < SYS_PRGS_SIZE)
	{
		if (!p_table[i])
			continue;
		pg = p_table[i];
		printf(
			"\033[31mPRG_DBG: %d / %p\033[0m\n"
			"\tPROC GROUP No_ %ld\n"
			"\tPGID: %d\n"
			"\tSTATE: %s\n"
			"\tMODE: %s\n"
			"\tPIDS: "
			, hot_sbsh, &hot_sbsh, i, pg->pgid, states[pg->state], mode[pg->mode]);
		pids = pg->members;
		while (pids)
		{
			psd = (t_ps_d *)&pids->size;
			printf("%d:%c%c%c ", psd->pid, *(states[psd->state]), *(states[psd->state] + 1), \
			*(states[psd->state] + 2));
			pids = pids->next;
		}
		printf("\n\n");
	}
	return (0);
}

int			DBG_SYS_GLB(void)
{
	printf(
		"\033[31mGL_DBG: %d / %p\033[0m\n"
		"\tPPID: %d\n"
		"\tPID: %d\n"
		"\tADR: &sys_pipes=%p / &hot_gid=%p\n\n"
		, hot_sbsh, &hot_sbsh, getppid(), getpid(), &sys_pipes, &hot_gid);
	return (0);
}

