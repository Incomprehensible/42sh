/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rms.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 15:54:49 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/14 19:46:33 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rms.h"
#include "bltn.h"
#include "ft_dlist.h"

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
	t_extab		*cnt;

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
	int		i;
	t_avln	*node;

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
	if (env->cmds)
		ft_avl_tree_free(env->cmds);
	dstr_del(&g_pwd);
	dstr_del(&g_oldpwd);
}

void			free_safe(void *ptr)
{
	if (ptr)
		free(ptr);
}
