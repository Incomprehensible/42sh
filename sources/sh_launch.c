/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 04:39:52 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/17 17:55:17 by hgranule         ###   ########.fr       */
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
	g_input_file = -1;
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
