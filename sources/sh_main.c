/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/28 13:47:23 by hgranule         ###   ########.fr       */
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

static t_env	g_m_env;
static t_opt	g_m_opt;

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

int				ft_isodigit(int ch)
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

void			at_exit_clear(void)
{
	save_histr(&g_m_env);
	del_history_buf(&g_histr);
	et_rm_clear_env(&g_m_env);
	et_rm_clear_opt(&g_m_opt);
}

int				main(int ac, char **av, char **ev)
{
	t_env		*env;
	t_opt		*opt;

	env = &g_m_env;
	opt = &g_m_opt;
	atexit(at_exit_clear);
	opt_init(ac, av, opt);
	env_init(ev, env);
	sys_var_init(env);
	sys_init(0);
	ft_avl_set(env->builtns, ft_avl_node("dbg_42", &bltn_dbg_snap, 8));
	ft_avl_set(env->locals, ft_avl_node("PS", \
	"%B%c2%t %c1%u%c4@%c5%H%c4:%c6%P%n%c3$> %c", 42));
	sh_launch(opt, env);
	return (0);
}
