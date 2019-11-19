/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_dbg_bltn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 19:10:28 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 14:39:30 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_dbg.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"
#include "fcntl.h"
#include "libft.h"
#include "stdio.h"

extern int		g_jid;

int				bltn_dbg_snap(char **args, ENV *envr)
{
	envr = 0;
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
	"    GINTRPT ......... %d\n"
	"    FD_TABLE_SIZE ... %d\n"
	"    PID ............. %d\n"
	"    PPID ............ %d\n"
	"    PGRP ............ %d\n"
	"    CWD ............. %s\n"
	, g_intr, getdtablesize(), getpid(), getppid(), getpgrp(), cwd_buff
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

void			DBG_PRINT_MATH(t_dlist *toklst)
{
	t_tok		*token;
	char		*msg;
	char		*value;

	msg = 0;
	while (toklst)
	{
		token = toklst->content;
		msg = token->type == OPRND ? "OPERAND" : msg;
		msg = token->type == HEX ? "HEX" : msg;
		msg = token->type == BIN ? "BINARY" : msg;
		msg = token->type == SEV ? "SEVEN" : msg;
		msg = token->type == DEC ? "DECIMAL" : msg;
		msg = token->type == INCRM ? "++" : msg;
		msg = token->type == DECRM ? "--" : msg;
		msg = token->type == NOT ? "~" : msg;
		msg = token->type == POSIT ? "POSITIVE" : msg;
		msg = token->type == NEGAT ? "NEGATIVE" : msg;
		msg = token->type == REJECT ? "!" : msg;
		msg = token->type == BIT_L ? "<<" : msg;
		msg = token->type == BIT_R ? ">>" : msg;
		msg = token->type == AND ? "&" : msg;
		msg = token->type == OR ? "|" : msg;
		msg = token->type == XOR ? "^" : msg;
		msg = token->type == MULT ? "*" : msg;
		msg = token->type == DEVID ? "/" : msg;
		msg = token->type == DEVREM ? "%" : msg;
		msg = token->type == PLUS ? "+" : msg;
		msg = token->type == MINUS ? "-" : msg;
		msg = token->type == EQU ? "=" : msg;
		msg = token->type == IS_EQU ? "==" : msg;
		msg = token->type == PLUS_EQ ? "+=" : msg;
		msg = token->type == MIN_EQ ? "-=" : msg;
		msg = token->type == NO_EQU ? "!=" : msg;
		msg = token->type == MORE_EQ ? ">=" : msg;
		msg = token->type == LESS_EQ ? "<=" : msg;
		msg = token->type == MORE ? ">" : msg;
		msg = token->type == LESS ? "<" : msg;
		msg = token->type == LOG_AND ? "&&" : msg;
		msg = token->type == LOG_OR ? "||" : msg;
		msg = token->type == INTO_BR ? "(" : msg;
		msg = token->type == OUT_BR ? ")" : msg;
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
	char		*states[] = {"NULL", "Done", "Terminated", "Suspended", "Running"};
	char		*mode[] = {"NULL", "Foreground", "Background"};
	t_dlist		*pids;

	i = g_jid;
	while (i > 0)
	{
		--i;
		if (!g_ptab[i])
			continue;
		pg = g_ptab[i];
		printf(
			"\n\033[31;7mSBSH_INFO: %d / %p\n\033[0m"
			"    [%ld] %d %-11s \t%s\n"
			"        %s\t\ts=%d\n"
			"     ----------------------------------------------- \n"
			, g_hsh, &g_hsh, i, pg->pgid, states[pg->state], pg->input_line, mode[pg->mode], pg->signal);
		pids = pg->members;
		while (pids)
		{
			psd = (t_ps_d *)&pids->size;
			printf(
				"    ->  \033[33;1m%d\033[0m %-11s e:s=[%hhu,%hhd]\t%s\n", \
				psd->pid, states[psd->state], \
				psd->exit_st, psd->signal, (char *)pids->content
			);
			pids = pids->next;
		}
		printf("\n");
	}
	return (0);
}

int			DBG_SYS_GLB(void)
{
	printf(
		"\033[31mGL_DBG: %d / %p\033[0m\n"
		"\tJID: %d\n"
		"\tPPID: %d\n"
		"\tPID: %d\n"
		"\tADR: &g_pipes=%p / &g_hgid=%p\n\n"
		, g_hsh, &g_hsh, g_jid, getppid(), getpid(), &g_pipes, &g_hgid);
	return (0);
}
