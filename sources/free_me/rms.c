/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rms.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 15:54:49 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/27 15:12:44 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rms.h"

void			free_token(void *tok, size_t sz)
{
	t_tok	*token;

	token = tok;
	sz = 0;
	if (token->value)
		free(token->value);
	free(token);
}

void			et_rm_rdr(void *rdir, size_t type)
{
	REDIRECT	*redir;

	redir = rdir;
	type = 0;
	if (redir)
	{
		if (redir->file)
			free((void *)redir->file);
		free((void *)redir);
	}
}

void			et_rm_warr(char **args)
{
	size_t		i;

	i = -1;
	while (args[++i])
	{
		free((void *)args[i]);
	}
	free((void *)args);
}

void			et_rm_expr(EXPRESSION *expr)
{
	t_dlist		*redirs;

	if (expr)
	{
		if ((redirs = expr->redirections))
			ft_dlst_delf(&redirs, (size_t)-1, et_rm_rdr);
		if (expr->args)
			et_rm_warr(expr->args);
		free((void *)expr);
	}
}

void			et_rm_ett(void *et_cont, size_t type)
{
	ETAB		*cnt;

	if (!(cnt = et_cont))
		return ;
	if (type == ET_EXPR)
		et_rm_expr((EXPRESSION *)et_cont);
	else if (type == ET_PIPE)
		free(et_cont);
}
