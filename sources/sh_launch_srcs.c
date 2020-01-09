/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch_srcs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 17:49:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/17 18:06:58 by hgranule         ###   ########.fr       */
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
#include "sys_tools/sys_dbg.h"
#include "ft_lbuffer.h"
#include "sh_tokenizer.h"

static int		sh_do_src_loop(int fd, DSTRING **line, \
ENV *env, t_dlist *toks[2])
{
	int				status;

	g_input_file = fd;
	while ((status = get_next_line(fd, line)) > 0)
	{
		while (reg_expr(line, 0, env))
			;
		if (sh_tokenizer((*line)->txt, toks) <= 0 && (g_input_nover = -1))
		{
			dstr_del(line);
			continue ;
		}
		dstr_del(line);
		sh_tparse(toks[0], env, TK_EOF, &status);
		ft_dlst_delf(toks, 0, free_token);
	}
	g_input_file = -1;
	return (0);
}

int				sh_do_src(char *filename, ENV *env)
{
	t_dyn_string	*line;
	int				fd;
	t_avl_tree		*avl;
	t_dlist			*toks[2];

	line = 0;
	if (!(sys_touch_file(filename) & SYS_TCH_F))
		return (sys_error_handler(filename, E_FOPEF, env));
	if ((fd = sys_file_op(filename, env, O_RDONLY, filename)) < 0)
		return (-1);
	ft_bzero(toks, sizeof(t_dlist *) * 2);
	avl = env->cmds;
	env->cmds = cmden_tree_create(env);
	sh_do_src_loop(fd, &line, env, toks);
	ft_avl_tree_free(env->cmds);
	env->cmds = avl;
	line ? dstr_del(&line) : 0;
	return (0);
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

int				sh_libs_enbl(t_opt *opt, ENV *env)
{
	t_dlist		*lib_lst;

	lib_lst = opt->lib_fs;
	while (lib_lst)
	{
		if (lib_lst->content == NULL)
			sys_perror("lib argument: failed to get arg (NULL)", 46, env);
		else if (ft_strlen(lib_lst->content) == 0)
			sys_error_handler("lib argument", E_ARGZL, env);
		else
			sh_do_src(lib_lst->content, env);
		lib_lst = lib_lst->next;
	}
	return (0);
}
