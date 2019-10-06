/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/06 23:42:57 by hgranule         ###   ########.fr       */
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


#define UT_TOK_INIT()		t_dlist *tokens = 0;\
							t_tok	token

#define UT_TOK_CR(typ, str)	token.type = typ; \
							if (str == 0) token.value = 0; \
							else token.value = ft_strdup(str); \
							ft_dlstpush(&tokens, \
							ft_dlstnew(&token, sizeof(t_tok)))

#define UT_TOK_END()		ft_dlst_delf(&tokens, (size_t)-1, free_token)

#define UT_TOK				tokens

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

static void		sh_loop(ENV *env)
{
	DSTRING		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		line = sh_readline(env);
		sys_write_history(line, env);
		if (line->txt && ft_strcmp(line->txt, "exit\n") == 0)
		{
			dstr_del(&line);
			break ;
		}
		ft_putendl(line->txt);
		dstr_del(&line);
	}
}

int				main(const int argc, char **argv, char **envp)
{
	ENV			env;
	int			status;
	DSTRING		*dstr;
	
	env_init(argc, argv, envp, &env);
	sys_var_init(&env, argv, argc);
	sys_init();

	ft_avl_set(env.builtns, ft_avl_node_cc("echo", &bltn_echo, 8));

	UT_TOK_INIT();

/* TEST
	UT_TOK_CR(TK_FUNCTION, 0);
	UT_TOK_CR(TK_NAME, "funct");

	UT_TOK_CR(TK_FOR, 0);
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_MATH, "a=0; a<10; ++a");
	UT_TOK_CR(TK_SEP, 0);
	UT_TOK_CR(TK_DO, 0);

	UT_TOK_CR(TK_IF, 0);
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_MATH, "a % 2 == 0");
	UT_TOK_CR(TK_SEP, 0);

	UT_TOK_CR(TK_THEN, 0);

	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "echo");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_DEREF, 0);
	UT_TOK_CR(TK_NAME, "1");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "KO");
	UT_TOK_CR(TK_SEP, 0);

	UT_TOK_CR(TK_ELSE, 0);

	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "echo");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_DEREF, 0);
	UT_TOK_CR(TK_NAME, "2");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "OK");
	UT_TOK_CR(TK_SEP, 0);

	UT_TOK_CR(TK_FI, 0);

	UT_TOK_CR(TK_IF, 0);
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_MATH, "a == 5");
	UT_TOK_CR(TK_SEP, 0);

	UT_TOK_CR(TK_THEN, 0);

	UT_TOK_CR(TK_BREAK, 0);

	UT_TOK_CR(TK_FI, 0);

	UT_TOK_CR(TK_DONE, 0);

	UT_TOK_CR(TK_FEND, 0);

	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "echo");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "HELLO");
	UT_TOK_CR(TK_SEP, 0);

	UT_TOK_CR(TK_EXPR, "funct");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "LOL");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_EXPR, "NELOL");

	UT_TOK_CR(TK_EOF, 0);
** END OF TEST*/

	UT_TOK_CR(TK_SUBSH, "ls; ls; ls;");
	UT_TOK_CR(TK_SEP, 0);

	UT_TOK_CR(TK_EXPR, "echo");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_DEREF, 0);
	UT_TOK_CR(TK_NAME, "?");
	UT_TOK_CR(TK_EOF, 0);

	sh_tparse(UT_TOK, &env, TK_EOF, &status);
	UT_TOK_END();

	
	// sh_loop(&env);
	// TERMINATE
	et_rm_clear_env(&env);
	return (0);
}

