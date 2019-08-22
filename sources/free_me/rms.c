/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rms.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 15:54:49 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/22 23:32:00 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rms.h"

void		et_rm_rdr(void *rdir)
{
	REDIRECT	*redir;

	redir = rdir;
	if (redir)
	{
		if (redir->file)
			free((void *)redir->file);
		free((void *)redir);
	}
}

void		et_rm_warr(char **args)
{
	size_t		i;

	i = -1;
	while (args[++i])
	{
		free((void *)args[i]);
	}
	free((void *)args);
}

void		et_rm_expr(EXPRESSION *expr)
{
	t_dlist	*redirs;

	if (expr)
	{
		if ((redirs = expr->redirections))
			ft_dlst_delf(&redirs, (size_t)-1, et_rm_rdr);
		if (expr->args)
			et_rm_warr(expr->args);
		free((void *)expr);
	}
}

void		et_rm_ett(void *et_cont)
{
	ETAB		*cnt;

	if (!(cnt = et_cont))
		return ;
	if (cnt->type == ET_EXPR)
		et_rm_expr(cnt->instruction);
}
