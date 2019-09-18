/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/18 21:32:31 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "env.h"
#include "executer.h"
#include "dstring.h"
#include "bltn.h"

#include <stdio.h>

#define UT_TOK_INIT()		t_dlist *tokens = 0;\
							t_tok	token

#define UT_TOK_CR(typ, str)	token.type = typ; \
							if (str == 0) token.value = 0; \
							else token.value = ft_strdup(str); \
							ft_dlstpush(&tokens, \
							ft_dlstnew(&token, sizeof(t_tok)))

#define UT_TOK_END()		ft_dlst_delf(&tokens, (size_t)-1, free_token)

#define UT_TOK				tokens

char			*sh_readline(void);
int				sh_tokenizer(char *str, t_dlist **token_list);
int				sh_tparser(t_dlist **token_list);

static void		sh_loop(void)
{
	char		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		//line = sh_readline();
		//sh_tokenizer(line, token_list);
		//sh_tparser(token_list);
		break ;
	}
}

// int				bltn_exit(char **args, ENV *envr)
// {
// 	exit(100);
// }

/*
!! TEMPORARY FUNCTION
!! Soon will be changed!
*/
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

int				main(const int argc, char **argv, char **envp)
{
	ENV			env;
	int			status;
	DSTRING		*dstr;
	
	env_init(argc, argv, envp, &env);
	sys_init();
	dstr = dstr_new("RAINBOWDASH");
	env_set_variable("MLP", dstr, &env);
	dstr_del(&dstr);
	ft_avl_set(env.builtns, ft_avl_node_cc("echo", &bltn_echo, 8));

	UT_TOK_INIT();
	UT_TOK_CR(TK_EXPR, "cat");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "-e");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_PROC_OUT, "ls -l");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_PROC_OUT, "ls -l");
	UT_TOK_CR(TK_SEP, 0);
	UT_TOK_CR(TK_EOF, 0);

	sh_tparse(UT_TOK, &env, TK_EOF, &status);

	UT_TOK_END();
	et_rm_clear_env(&env);
	// TERMINATE
	return (0);
}
/*
	printf(
		"\n================ 42sh ====\n\n"
		"LPID   = %15d ]\n"
		"PID    = %15d ]\n"
		"STATUS = %15d ]\n"
		"SIGNAL = %15d ]\n"
		"\n================= wait_cps\n\n",
		last_child, pid, *status, signal);
*/