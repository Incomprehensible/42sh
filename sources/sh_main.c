/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/07 07:03:47 by bomanyte         ###   ########.fr       */
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
		if (get_line(0, &line) < 0)
		{
			ft_putendl("== Input failed ==");
			break ;
		}
		if (sh_tokenizer(line, token_list) <= 0)
		{
		    free(line);
            continue;
        }
		// while (token_list[0])
        // {
		//     printf("value %s type %zx\n", ((t_tok *)(token_list[0]->content))->value, ((t_tok *)(token_list[0]->content))->type);
		//     token_list[0] = token_list[0]->next;
        // }
		ft_putendl("== go to parsing ==");
		sh_tparse(token_list[0], env, TK_EOF, &status);
		printf("== GETPRG %d ==\n", tcgetpgrp(0));
		ft_dlst_delf(token_list, 0, free_token);
		free(line);
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

	sh_loop(&env);
	et_rm_clear_env(&env);
	return (0);
}

