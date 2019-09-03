/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/03 08:56:15 by hgranule         ###   ########.fr       */
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

int				bltn_echo(char **args, ENV *envr)
{
	size_t		i;

	i = 0;
	while (args[++i])
	{
		i != 1 ? ft_putstr(" ") : 0;
		ft_putstr(args[i]);
	}
	ft_putstr("<>\n");
	return (0);
}

int				main(const int argc, char **argv, char **envp)
{
	ENV			env;
	int			status;
	
	env_init(argc, argv, envp, &env);
	env_set_variable("a", dstr_new("150"), &env);
	ft_avl_set(env.builtns, ft_avl_node_cc("echo", &bltn_echo, 8));

	UT_TOK_INIT();
	UT_TOK_CR(math_tk, "a = 0");
	UT_TOK_CR(sep_tk, 0);
	UT_TOK_CR(expr_tk, "echo");
	UT_TOK_CR(empty_tk, 0);
	UT_TOK_CR(deref_tk, 0);
	UT_TOK_CR(name_tk, "a");
	UT_TOK_CR(sep_tk, 0);
	UT_TOK_CR(math_tk, "a = (25 + 67 + a + a) + a");
	UT_TOK_CR(sep_tk, 0);
	UT_TOK_CR(expr_tk, "echo");
	UT_TOK_CR(empty_tk, 0);
	UT_TOK_CR(deref_tk, 0);
	UT_TOK_CR(name_tk, "a");
	UT_TOK_CR(eof_tk, 0);

	sh_tparse(UT_TOK, &env, eof_tk, &status);

	UT_TOK_END();

	et_rm_clear_env(&env);
	// TERMINATE
	return (0);
}
