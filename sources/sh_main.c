/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/18 22:45:37 by gdaemoni         ###   ########.fr       */
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
#include "sys_tools/sys_token_to_str.h"

#include "sys_tools/sys_dbg.h"

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
	extern int	g_prompt_fd;

	ft_putstr_fd(prompt, g_prompt_fd);
	dstr = 0;
	status = get_next_line(0, &dstr);
	if (status == 0)
		return (0);
	if (status < 0)
	{
		sys_error_message("INPUT ERROR1 :(", 0);
		perror("INP");
		return (0);
	}
	line = ft_strdup(dstr->txt);
	if (line == 0)
		sys_error_message("INPUT ERROR2 :(", 0);
	dstr_del(&dstr);
	return (line);
}

void			add_buf_history(DSTRING *buf)
{
	int			ind;

	ind = S_DARR_STRINGS - (g_histr.count + 1);
	if (ind > 0)
	{
		if (buf->strlen && !(ft_strequ(buf->txt, "exit")))
		{
			g_histr.strings[ind] = dstr_nerr(buf->txt);
			g_histr.count++;
			g_histr.allsize += buf->strlen;
		}
	}
}

void			save_histr(ENV *envr)
{
	int		fd;
	int		ind;
	size_t	count;

	if (!g_histr.count || ((fd = get_history_fd \
	(O_WRONLY | O_TRUNC, "REWRITE_HISTORY: File error", envr)) < 0))
		return ;
	if (g_histr.count > 1000)
		ind = S_DARR_STRINGS - (g_histr.count - 1000);
	else
		ind = S_DARR_STRINGS - 1;
	count = 0;
	while (count <= 1001 && g_histr.strings[ind])
	{
		write(fd, g_histr.strings[ind]->txt, g_histr.strings[ind]->strlen);
		write(fd, "\n", 1);
		ind--;
		count++;
	}
	close(fd);
	// free_darr_re(g_histr.strings, g_histr.count);
}

void			del_history_buf(t_darr *histr)
{
	int		i;
	size_t	count;	

	count = 0;
	i = S_DARR_STRINGS - histr->count - 1;
	while (count < histr->count)
	{
		++count;
		dstr_del(&(histr->strings[i--]));
	}
}

size_t			args_len(char **argv)
{
	size_t		i;

	i = 0;
	while (argv && argv[i])
	{
		++i;
	}
	return (i);
}

// !! TEMPORARY FUNCTIONS ===================================================================== !!
// !! Soon will be changed! =================================================================== !!

void			sh_loop(ENV *env)
{
	DSTRING		*line;
	DSTRING		*prompt;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;
	int			status;
	char        code;

	init_histr(env);
	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	code = 0;
	while (1)
	{
		if (!(prompt = sys_get_prompt_num(env, code)))
			sys_fatal_memerr("PROMPT FAILED");
		if (!(line = sh_readline(prompt, env)))
		{
			ft_putendl("== Input failed or ENDED ==");
			break ;
		}
		add_buf_history(line);
		save_histr(env);
		if (sh_tokenizer(line->txt, token_list) <= 0)
		{
			dstr_del(&prompt);
		    dstr_del(&line);
		    code = get_code();
		    INPUT_NOT_OVER = -1;
            continue ;
        }
		code = 0;
		dstr_del(&prompt);
		dstr_del(&line);
		if (dbg_tok_pr_flag)
			DBG_PRINT_TOKENS(token_list[0]);
		g_intr = 0;
		sh_tparse(token_list[0], env, TK_EOF, &status);
		ft_dlst_delf(token_list, 0, free_token);
	}
}

#define _TEST_PS "%B%c1%?%c%B%%%c4%P %c3$ %c"

int				main(const int argc, char **argv, char **envp)
{
	ENV				env;
	char			**lflags = 0;
	char			*flags = 0;

	if (sys_argv_parse(argv, &lflags, &flags))
		sys_fatal_memerr("ARGV_PRS_FAILED");

	env_init(argc, argv, envp, &env);
	sys_var_init(&env, argv, argc);
	sys_init(0);

	// temp bltns
	ft_avl_set(env.builtns, ft_avl_node_cc("dbg_42", &bltn_dbg_snap, 8));
	ft_avl_set(env.builtns, ft_avl_node_cc("source", &bltn_source, 8));
	ft_avl_set(env.globals, ft_avl_node_cc("CLICOLOR", "1", 2));
	ft_avl_set(env.locals, ft_avl_node_cc("PS", _TEST_PS, ft_strlen(_TEST_PS) + 1));
	ft_avl_set(env.globals, ft_avl_node_cc("GREP_OPTIONS", "--color=auto", 14));

	// CHOOSE A TYPE OF LAUNCH WITH lflags n flags
	sh_launch_system(&env, lflags, flags, argv);

	et_rm_clear_env(&env);
	if (lflags)
		et_rm_warr(lflags);
	if (flags)
		free(flags);
	return (0);
}


// #include <fcntl.h>

// int main()
// {
// 	char	name[] = "/Users/gdaemoni/Desktop/test_histr.txt";
// 	int		fd;
// 	int		num = 0;

// 	fd = open(name, O_RDWR | O_CREAT);

// 	while (num < 2500)
// 	{
// 		char *tmp = ft_itoa(num);
// 		write(fd, tmp, ft_strlen(tmp));
// 		write(fd, "\n", 1);
// 		free(tmp);
// 		num++;
// 	}
// 	return (1);
// }