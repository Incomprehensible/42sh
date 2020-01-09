/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rms_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 18:11:17 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/29 21:31:04 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rms.h"
#include "bltn.h"
#include "ft_dlist.h"

void			et_rm_prc(void *ptr, size_t sz)
{
	if (ptr)
		free(ptr);
	sz = 0;
}

void			et_rm_func(void *func)
{
	t_dlist		*toks;
	t_func		*func_ptr;

	func_ptr = func;
	if (func_ptr)
	{
		toks = func_ptr->func_code;
		ft_dlst_delf(&toks, (size_t)-1, free_token);
		free(func);
	}
}

void			free_token(void *tok, size_t sz)
{
	t_tok	*token;

	if (!(token = tok))
		return ;
	sz = 0;
	if (token->value)
		free(token->value);
	free(token);
}

void			et_rm_rdr(void *rdir, size_t type)
{
	t_redir		*redir;

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
