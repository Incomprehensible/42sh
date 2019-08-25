/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/25 03:34:36 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "env.h"
#include "executer.h"
#include "dstring.h"

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

void			free_token(void *tok, size_t sz)
{
	t_tok	*token;

	token = tok;
	sz = 0;
	if (token->value)
		free(token->value);
	free(token);
}

int				bltn_exit(char **args, ENV *envr)
{
	exit(100);
}

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
	ENV			enviroment;
	char		*pathes = "/Users/hgranule/.brew/bin:"
	"/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:"
	"/Library/Frameworks/Mono.framework/Versions/Current/Commands:/usr/local/munki";

	enviroment.globals = ft_avl_tree_create(free);
	ft_avl_set(enviroment.globals, ft_avl_node("PATH", pathes, ft_strlen(pathes) + 1));

	enviroment.locals = 0;
	enviroment.builtns = ft_avl_tree_create(0);
	ft_avl_set(enviroment.builtns, ft_avl_node_cc("exit", bltn_exit, sizeof(bltn_exit)));
	ft_avl_set(enviroment.builtns, ft_avl_node_cc("echo", bltn_echo, sizeof(bltn_exit)));
	enviroment.funcs = 0;



	UT_TOK_INIT();
	UT_TOK_CR(expr_tk, "echo");
	UT_TOK_CR(expr_tk, "THIS IS FOR PASHA");
	UT_TOK_CR(eof_tk, 0);

	sh_tparse(UT_TOK, &enviroment);

	UT_TOK_END();
	ft_avl_tree_free(enviroment.globals);
	ft_avl_tree_free(enviroment.builtns);

	// TERMINATE
	return (0);
}
