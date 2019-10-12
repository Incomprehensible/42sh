/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/12 02:18:35 by hgranule         ###   ########.fr       */
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

int				bltn_echo(char **args, ENV *envr)
{
	size_t		i;

	i = 0;
	while (args[++i])
	{
		i != 1 ? ft_putstr(" ") : 0;
		ft_putstr(args[i]);
	}
	ft_putstr(" @\n");
	return (0);
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
		value = token->value ? token->value : "<->";
		printf("%5s: %s\n", msg, value);
		toklst = toklst->next;
	}
}

// !! TEMPORARY FUNCTIONS ===================================================================== !!
// !! Soon will be changed! =================================================================== !!

static void		sh_loop(ENV *env)
{
	char		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;
	int			status;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		ft_putstr("42SH$ ");
		if (get_line(0, &line) <= 0)
		{
			ft_putendl("== Input failed or ENDED ==");
			break ;
		}
		if (sh_tokenizer(line, token_list) <= 0)
		{
		    free(line);
            continue;
        }
		// DBG_PRINT_TOKENS(token_list[0]);
		// ft_putendl("== go to parsing ==");
		sh_tparse(token_list[0], env, TK_EOF, &status);
		ft_dlst_delf(token_list, 0, free_token);
		free(line);
	}
}

int				main(const int argc, char **argv, char **envp)
{
	ENV				env;
	int				status;
	DSTRING			*dstr;
	extern pid_t	hot_sbsh;
	
	env_init(argc, argv, envp, &env);
	sys_var_init(&env, argv, argc);
	sys_init();
	hot_sbsh = 0;

	// temp bltns
	ft_avl_set(env.builtns, ft_avl_node_cc("echo", &bltn_echo, 8));
	ft_avl_set(env.builtns, ft_avl_node_cc("dbg_42", &bltn_dbg_snap, 8));
	ft_avl_set(env.builtns, ft_avl_node_cc("fg", &bltn_fg, 8));

	sh_loop(&env);
	et_rm_clear_env(&env);
	return (0);
}
