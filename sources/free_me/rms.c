/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rms.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 15:54:49 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 15:24:25 by fnancy           ###   ########.fr       */
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
	FUNC		*func_ptr;

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

void			et_rm_sbsh(SUBSH *sbh)
{
	t_dlist		*redir;

	if (sbh)
	{
		if ((redir = sbh->redirections))
			ft_dlst_delf(&redir, (size_t)-1, et_rm_rdr);
		if (sbh->commands)
			free(sbh->commands);
		free((void *)sbh);
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
	else if (type == ET_SUBSH)
		et_rm_sbsh((SUBSH *)et_cont);
	else if (type == ET_BCKGR)
		free(et_cont);
}

void			et_rm_clear_env(ENV *env)
{
	int i;
	t_avln *node;

	i = -1;
	while (++i < bltn_cnt_builtins())
	{
		node = ft_avl_search(env->builtns, g_bltns_str[i]);
		node->content = NULL;
	}
	if (env->globals)
		ft_avl_tree_free(env->globals);
	if (env->locals)
		ft_avl_tree_free(env->locals);
	if (env->builtns)
		ft_avl_tree_free(env->builtns);
	if (env->funcs)
		ft_avl_tree_free(env->funcs);
	if (env->aliases)
		ft_avl_tree_free(env->aliases);
	if (env->core)
		ft_avl_tree_free(env->core);
	dstr_del(&g_pwd);
	dstr_del(&g_oldpwd);
}