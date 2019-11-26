/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/26 20:01:54 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "sh_readline.h"
#include "env.h"
#include "executer.h"
#include "parser.h"
#include "dstring.h"
#include "bltn.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_token_to_str.h"
#include "sys_tools/sys_dbg.h"
#include <sys/ioctl.h>
#include "sh_tokenizer.h"
#include "sh_options.h"

// !! TEMPORARY FUNCTIONS ===================================================================== !!
// !! Soon will be changed! =================================================================== !!

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
	sys_core_set_init(envr, args + 1);
	if (!(SYS_TCH_F & sys_touch_file(args[1])) && (status = 2))
		sys_error_handler(args[1], E_FOPEF, envr);
	else
		status = sh_do_src(args[1], envr);
	ft_avl_tree_free(envr->core);
	envr->core = tmp;
	return (status);
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

/*
** NEEDED IN ECHO
*/
int		ft_isodigit(int ch)
{
	if ('0' <= ch && ch <= '8')
		return (1);
	return (0);
}

void			put_dlist_str(t_dlist *lst)
{
	while (lst)
	{
		ft_putstr("> ");
		ft_putendl(lst->content);
		lst = lst->next;
	}
}

void			put_ops(t_opt opt)
{
	ft_putendl("LIBS:");
	if (opt.lib_fs)
		put_dlist_str(opt.lib_fs);
	ft_putendl("OPS:");
	if (opt.ops)
		put_dlist_str(opt.ops);
	ft_putendl("PARAMS:");
	if (opt.params)
		put_dlist_str(opt.params);
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

// %t %u@%h :: %p :: %n

#define _TEST_PS "%B%c2%t %c1%u%c4@%c5%H%c4:%c6%P%n%c3$> %c"

// int				main(int argc, char **argv, char **envp)
// {
// 	ENV				env;
// 	char			**lflags = 0;
// 	char			*flags = 0;

// 	argc = 0;
// 	if (sys_argv_parse(argv, &lflags, &flags))
// 		sys_fatal_memerr("ARGV_PRS_FAILED");

// 	env_init(envp, &env);
// 	sys_var_init(&env);
// 	sys_init(0);

// 	// temp bltns
// 	ft_avl_set(env.builtns, ft_avl_node_cc("dbg_42", &bltn_dbg_snap, 8));
// 	ft_avl_set(env.builtns, ft_avl_node_cc("source", &bltn_source, 8));
// 	ft_avl_set(env.globals, ft_avl_node_cc("CLICOLOR", "1", 2));
// 	ft_avl_set(env.locals, ft_avl_node_cc("PS", _TEST_PS, ft_strlen(_TEST_PS) + 1));
// 	ft_avl_set(env.globals, ft_avl_node_cc("GREP_OPTIONS", "--color=auto", 14));

// 	// CHOOSE A TYPE OF LAUNCH WITH lflags n flags
// 	sh_launch_system(&env, flags, argv);

// 	et_rm_clear_env(&env);
// 	if (lflags)
// 		et_rm_warr(lflags);
// 	if (flags)
// 		free(flags);
// 	return (0);
// }

int				sh_do_str(ENV *env, char *str)
{
	int			status;
	t_dlist		*toks[2];
	int			wtf;

	if (!str)
		return (-1);
	ft_bzero(toks, sizeof(t_dlist *) * 2);
	if ((wtf = sh_tokenizer(str, toks)) <= 0)
		return (wtf);
	sh_tparse(toks[0], env, TK_EOF, &status);
	if (toks[0])
		ft_dlst_delf(toks, 0, free_token);
	return (0);
}

int				sh_do_src(char *filename, ENV *env)
{
	DSTRING		*line;
	int			fd;
	int			status;

	line = 0;
	if ((fd = sys_file_op(filename, env, O_RDONLY, filename)) < 0)
		return (-1);
	while ((status = get_next_line(fd, &line)) > 0)
	{
		sh_do_str(env, line->txt);
		dstr_del(&line);
		line = 0;
	}
	if (line)
		dstr_del(&line);
	return (0);
}

size_t			ft_dlst_len(t_dlist *lst)
{
	size_t		len;

	len = 0;
	while (lst && ++len)
		lst = lst->next;
	return (len);
}

char			**ft_dlst_to_warr(t_dlist *lst, \
char *(*cnt_cb)(void *, size_t sz))
{
	size_t		len;
	size_t		i;
	char		**warr;

	len = ft_dlst_len(lst);
	i = 0;
	warr = ft_memalloc((len + 1) * sizeof(char *));
	if (!warr)
		return (NULL);
	while (lst)
	{
		warr[i] = cnt_cb(lst->content, lst->size);
		++i;
		lst = lst->next;
	}
	warr[i] = NULL;
	return (warr);
}

char			*callback_dlst_warr(void *cnt, size_t sz)
{
	sz = 0;
	return ((char *)cnt);
}

int				sh_launch_file(t_opt *opt, ENV *env)
{
	char		**argv;
	size_t		len;
	int			status;

	len = ft_dlst_len(opt->params);
	if (opt->params && (sys_touch_file(opt->params->content) & SYS_TCH_F))
	{
		argv = ft_dlst_to_warr(opt->params, callback_dlst_warr);
		if (!argv)
			sys_fatal_memerr("ARG_CORE_FAILED");
		sys_core_set_init(env, argv);
		status = sh_do_src(opt->params->content, env);
		free(argv);
		return (status);
	}
	sys_error_handler(opt->params ? \
	opt->params->content : 0, E_FOPEF, env);
	return (-1);
}

int				sh_launch_lines(t_opt *opt, ENV *env)
{
	
}

int				main(int ac, char **av, char **ev)
{
	ENV			env;
	t_opt		opt;

	opt_init(ac, av, &opt);
	env_init(ev, &env);
	sys_var_init(&env);
	sys_init(0);

	ft_avl_set(env.builtns, ft_avl_node_cc("source", &bltn_source, 8));

	// put_ops(opt);
	sh_launch_file(&opt, &env);

	et_rm_clear_env(&env);
	et_rm_clear_opt(&opt);
	return (0);
}