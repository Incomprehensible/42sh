/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/13 05:48:24 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "sh_readline.h"
#include "env.h"
#include "executer.h"
#include "dstring.h"
#include "bltn.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"

#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "sh_tokenizer.h"

// !! TEMPORARY FUNCTIONS ===================================================================== !!
// !! Soon will be changed! =================================================================== !!

#define UT_TOK_INIT()		t_dlist *tokens = 0;\
							t_tok	token
#define UT_TOK_CR(typ, str)	token.type = typ; \
							if (str == 0) token.value = 0; \
							else token.value = ft_strdup(str); \
							ft_dlstpush(&tokens, \
							ft_dlstnew(&token, sizeof(t_tok)))
#define UT_TOK_END()		ft_dlst_delf(&tokens, (size_t)-1, free_token)
#define UT_TOK				tokens

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

int				bltn_fg(char **args, ENV *envr)
{
	if (!args[1])
	{
		ssize_t		a = -1;
		
		while (++a < SYS_PRGS_SIZE)
		{
			if (p_table[a] && p_table[a]->mode == PS_M_BG && p_table[a]->state == PS_S_STP)
			{
				t_dlist		*pd_lst = p_table[a]->members;
				t_ps_d		*psd;

				while (pd_lst)
				{
					psd = (t_ps_d *)&pd_lst->size;
					if (psd->state == PS_S_STP)
						psd->state = PS_S_RUN;
					pd_lst = pd_lst->next;
				}
				p_table[a]->state = PS_S_RUN;
				p_table[a]->mode = PS_M_FG;
				break ;
			}
		}
		if (a == SYS_PRGS_SIZE)
			return (2);
		tcsetpgrp(0, p_table[a]->pgid);
		killpg(p_table[a]->pgid, SIGCONT);
		return (0);
	}
	return (1);
}

int				bltn_dbg_snap(char **args, ENV *envr)
{
	if (args[1])
	{
		if (ft_strequ(args[1], "ps"))
			DBG_SYS_SNAP();
		if (ft_strequ(args[1], "gl"))
			DBG_SYS_GLB();
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

int				bltn_source(char **args, ENV *envr)
{
	t_avl_tree	*tmp;
	int			status;

	if (args[1] == 0)
	{
		sys_error_message("source: argument file required\n"
		"Usage: source filename [arguments]", 0);
		return (2);
	}
	tmp = envr->core;
	if (!(envr->core = ft_avl_tree_create(free)))
	{
		sys_error_message("source: Failed allocation", 0);
		envr->core = tmp;
		return (255);
	}
	sys_core_set_init(envr, args);
	status = sh_launch_file(envr, args[1]);
	ft_avl_tree_free(envr->core);
	envr->core = tmp;
	return (status);
}

char			*tmp_readline(char *prompt)
{
	DSTRING		*dstr;
	char		*line;
	int			status;

	ft_putstr(prompt);
	dstr = 0;
	status = get_next_line(0, &dstr);
	if (status == 0)
		return (0);
	if (status < 0)
	{
		sys_error_message("INPUT ERROR :(", 0);
		return (0);
	}
	line = ft_strdup(dstr->txt);
	if (line == 0)
		sys_error_message("INPUT ERROR :(", 0);
	dstr_del(&dstr);
	return (line);
}

// !! TEMPORARY FUNCTIONS ===================================================================== !!
// !! Soon will be changed! =================================================================== !!

void			sh_loop(ENV *env)
{
	char		*line;
	DSTRING		*prompt;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;
	int			status;
	char        code;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	code = 0;
	while (1)
	{
		if (!(prompt = sys_get_prompt_num(env, code)))
			sys_fatal_memerr("PROMPT FAILED");
		if (!(line = tmp_readline(prompt->txt)))
		{
			ft_putendl("== Input failed or ENDED ==");
			break ;
		}
		if (sh_tokenizer(line, token_list) <= 0)
		{
			dstr_del(&prompt);
		    free(line);
		    code = get_code();
		    INPUT_NOT_OVER = -1;
            continue ;
        }
		code = 0;
		dstr_del(&prompt);
		free(line);
		sh_tparse(token_list[0], env, TK_EOF, &status);
		ft_dlst_delf(token_list, 0, free_token);
	}
}

int				main(const int argc, char **argv, char **envp)
{
	ENV				env;
	char			**lflags = 0;
	char			*flags = 0;

	if (sys_argv_parse(argv, &lflags, &flags))
		sys_fatal_memerr("ARGV_PRS_FAILED");

	env_init(argc, argv, envp, &env);
	sys_var_init(&env, argv, argc);
	sys_init();

	// temp bltns
	ft_avl_set(env.builtns, ft_avl_node_cc("dbg_42", &bltn_dbg_snap, 8));
	ft_avl_set(env.builtns, ft_avl_node_cc("fg", &bltn_fg, 8));
	ft_avl_set(env.builtns, ft_avl_node_cc("source", &bltn_source, 8));

	// CHOOSE A TYPE OF LAUNCH WITH lflags n flags
	sh_launch_system(&env, lflags, flags, argv);

	et_rm_clear_env(&env);
	if (lflags)
		et_rm_warr(lflags);
	if (flags)
		free(flags);
	return (0);
}
