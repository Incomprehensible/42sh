/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch_support.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 14:58:53 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 15:03:21 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "ft_io.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_token_to_str.h"

extern pid_t	g_hgid;
extern pid_t	g_hsh;
extern char		*g_hbg;
ENV				*g_env;

int				prs_execute_expr(ETAB **etab, ENV *envs)
{
	t_extab		*etab_row;
	t_extab		*pipe_free;
	int			status;
	pid_t		cpid;
	char		*i_line;

	etab_row = 0;
	pipe_free = 0;
	status = 127;
	while (*etab && ((*etab)->type == ET_EXPR || (*etab)->type == ET_PIPE \
	|| (*etab)->type == ET_SUBSH || (*etab)->type == ET_BCKGR))
	{
		i_line = sys_get_str_from_etab(etab);
		etab_row = (ETAB *)ft_dlstshift((t_dlist **)etab);
		cpid = prs_etab_handlers(&etab_row, &pipe_free, &status, envs);
		if (cpid > 0)
			pex_g_ptab_pgid(cpid, i_line);
		else if (i_line)
			free(i_line);
	}
	ft_dlst_delf((t_dlist **)&pipe_free, (size_t)-1, et_rm_ett);
	return (prs_post_execute_xp(&status, cpid, envs));
}

int				math_to_expr_maker(ETAB **etab)
{
	t_math		*math;
	t_command	*cmd;
	char		*tmp;

	math = (*etab)->instruction;
	tmp = math->expr;
	(*etab)->type = ET_EXPR;
	cmd = (EXPRESSION *)math;
	if (!(cmd->args = ft_memalloc(4 * sizeof(char *))))
		sys_fatal_memerr("MATH_BLTN_FAIL_XPR");
	cmd->args[0] = ft_strdup("((");
	cmd->args[2] = ft_strdup("))");
	if (!cmd->args[0] || !cmd->args[2])
		sys_fatal_memerr("MATH_BLTN_FAIL_XPR");
	cmd->args[1] = tmp;
	return (0);
}

int				prs_executor(ETAB **etab, ENV *envs)
{
	ETAB		*etab_row;
	int			status;

	while ((etab_row = *etab) != 0)
	{
		if (etab_row->type == ET_MATH)
			status = math_to_expr_maker(etab);
		if (etab_row->type == ET_EXPR || etab_row->type == ET_SUBSH || \
		etab_row->type == ET_BCKGR)
			status = prs_execute_expr(etab, envs);
	}
	return (status);
}
