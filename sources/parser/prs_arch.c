/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 04:13:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/25 03:26:56 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"
#include "ft_io.h"

int				prs_execute_expr(ETAB **etab ,ENV *envs)
{
	ETAB		*etab_row;
	ETAB		*pipe_free;
	int			status;
	pid_t		cpid;

	etab_row = 0;
	pipe_free = 0;
	cpid = 0;
	status = 127;
	while (*etab && ((*etab)->type == ET_EXPR || (*etab)->type == ET_PIPE))
	{
		etab_row = (ETAB *)ft_dlstshift((t_dlist **)etab);
		if (etab_row->type == ET_EXPR)
			cpid = exe_execute_expr(etab_row->instruction, envs, &status);
		if (etab_row->type == ET_PIPE)
			ft_dlstunshift((t_dlist **)&pipe_free, (t_dlist *)etab_row);
		else
			ft_dlst_delcut((t_dlist **)&etab_row, et_rm_ett);
	}
	ft_dlst_delf((t_dlist **)&pipe_free, (size_t)-1, et_rm_ett);
	if (cpid > 0)
		status = exe_wait_cps(cpid);
	return (status);
}

int				prs_executor(ETAB **etab, ENV *envs) //TODO! ERROR CHECKING NEED
{
	ETAB		*etab_row;
	ETAB		*trash;
	int			status;
	pid_t		cpid;

	while ((etab_row = *etab) != 0)
	{
		if (etab_row->type == ET_EXPR)
			status = prs_execute_expr(etab, envs);
	}
	return (status);
}

void			sh_tparse(t_dlist *tks, ENV *envs)
{
	ETAB		*etab;
	int			status;
	t_tok		*tok;

	etab = 0;
	while (tks && (tok = tks->content))
	{
		if (etab && (tok->type == sep_tk || tok->type == eof_tk))
		{
			status = prs_executor(&etab, envs); //TODO! ERROR CHEKING NEED
			ft_putstr("RETURNED STATUS: ");
			ft_putnbr(status);
			ft_putstr("\n");
		}
		tks = tok->type == eof_tk || tok->type == empty_tk || \
		 tok->type == sep_tk ? tks->next : tks;
		tks = tok->type == expr_tk ? prs_expr(&etab, tks) : tks;
		tks = tok->type == pipe_tk ? prs_pipe(&etab, tks) : tks;
	}
	if (etab)
		ft_dlst_delf((t_dlist **)&etab, (size_t)-1, et_rm_ett);
	return ; //! MALLOC CALL FAILED.
}
