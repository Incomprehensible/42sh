/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 04:39:52 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/14 00:46:50 by hgranule         ###   ########.fr       */
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

int				sh_do_src(char *filename, ENV *env)
{
	t_dyn_string	*line;
	int				fd;
	int				status;
	t_dlist			*toks[2];

	line = 0;
	if (!(sys_touch_file(filename) & SYS_TCH_F))
		return (sys_error_handler(filename, E_FOPEF, env));
	if ((fd = sys_file_op(filename, env, O_RDONLY, filename)) < 0)
		return (-1);
	ft_bzero(toks, sizeof(t_dlist *) * 2);
	env->cmds = cmden_tree_create(env);
	while ((status = get_next_line(fd, &line)) > 0)
	{
		if (sh_tokenizer(line->txt, toks) <= 0 && (g_input_nover = -1))
		{
			dstr_del(&line);
			continue ;
		}
		dstr_del(&line);
		sh_tparse(toks[0], env, TK_EOF, &status);
		ft_dlst_delf(toks, 0, free_token);
	}
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

void			loop_pre_conf(ENV *env, DSTRING **prompt)
{
	if (env->cmds)
	{
		ft_avl_tree_free(env->cmds);
		env->cmds = NULL;
	}
	env->cmds = cmden_tree_create(env);
	if (!((*prompt) = sys_get_prompt_num(env, get_code())))
		sys_fatal_memerr("PROMPT_ALLOCA_FATAL");
}

int				sh_launch_loop(ENV *env)
{
	t_dyn_string	*line;
	t_dyn_string	*prompt;
	t_dlist			*token_list[2];
	int				status;
	short			tk_status;

	init_histr(env);
	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		loop_pre_conf(env, &prompt);
		if (!(line = sh_readline(prompt, env)))
			break ;
		dstr_del(&prompt);
		add_buf_history(line);
		tk_status = sh_tokenizer(line->txt, token_list);
		dstr_del(&line);
		if (!(g_intr = 0) && tk_status <= 0)
			continue ;
		sh_tparse(token_list[0], env, TK_EOF, &status);
		ft_dlst_delf(token_list, 0, free_token);
	}
	prompt ? dstr_del(&prompt) : 0;
	return (sys_perror("Input was closed. Exiting.", 0, env));
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
