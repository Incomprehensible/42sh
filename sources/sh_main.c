/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 19:59:12 by hgranule         ###   ########.fr       */
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

#include "sh_vars.h"
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

/*
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

	ft_avl_set(env.globals, ft_avl_node("PWD", (char*)"/var", 15));
	UT_TOK_INIT();
	UT_TOK_CR(TK_EXPR, "sdfsdfsdfsd");
	UT_TOK_CR(TK_SEP, 0);
	UT_TOK_CR(TK_EXPR, "echo");
	UT_TOK_CR(TK_EMPTY, 0);
	UT_TOK_CR(TK_DEREF, 0);
	UT_TOK_CR(TK_NAME, "?");
	UT_TOK_CR(TK_EOF, 0);

	sh_tparse(UT_TOK, &env, TK_EOF, &status);

	UT_TOK_END();
	et_rm_clear_env(&env);
	// TERMINATE
	return (0);
}
*/

/*
void		write_history(DSTRING *line)
{
	int		fd;

	if (line->strlen == 0 || ft_strcmp(line->txt, "exit()") == 0 \
			|| ft_strcmp(line->txt, "\n") == 0)
		return ;
	dstr_insert_ch(line, '\n', line->strlen);
	if ((fd = open(HISTORY_PATH, O_CREAT | O_EXCL, S_IREAD | S_IWRITE)) != -1)
		close(fd);
	else if ((fd = open(HISTORY_PATH, O_RDWR | O_APPEND)) == -1)
	{
		perror("\nopen failed on history command file");
        exit (1);
	}
	write(fd, line->txt, line->strlen);
	close(fd);
}
*/

// /*
static void		sh_loop(ENV *env)
{
	DSTRING		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		line = sh_readline(env);
		sys_write_history(line, env);
		if (line->txt && ft_strcmp(line->txt, "exit()") == 0)
		{
			dstr_del(&line);
			break ;
		}
		dstr_del(&line);
	}
}

int				main(const int argc, char **argv, char **envp)
{
	ENV			env;
	int			status;
	DSTRING		*dstr;
	
	env_init(argc, argv, envp, &env);
	sys_var_init(&env);
	sys_init();

	ft_avl_set(env.builtns, ft_avl_node_cc("echo", &bltn_echo, 8));
	// LOOP

	UT_TOK_INIT();

	UT_TOK_CR(TK_VAR, "PATH");
	UT_TOK_CR(TK_ASSIGM, "+");
	UT_TOK_CR(TK_VALUE, ":");
	UT_TOK_CR(TK_DEREF, 0);
	UT_TOK_CR(TK_NAME, "HOME");
	UT_TOK_CR(TK_VALUE, "/Desktop/my_own_bash");
	UT_TOK_CR(TK_EOF, 0);

	sh_tparse(UT_TOK, &env, TK_EOF, &status);

	UT_TOK_END();

	sh_loop(&env);
	// TERMINATE
	et_rm_clear_env(&env);
	return (0);
}
// */

// ls Libf[a-z]/*s/ft?a*
