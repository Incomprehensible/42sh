/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd_hash.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 23:11:10 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/14 19:37:21 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "sys_tools/sys_errors.h"
#include "rms.h"

char		*check_binary_file(char *path, char *name)
{
	char			*fpath;
	t_dyn_string	*buf;
	struct stat		st;

	buf = dstr_new(path);
	fpath = NULL;
	dstr_insert_ch(buf, '/', buf->strlen);
	dstr_insert_str(buf, name, buf->strlen);
	if (!buf)
		sys_fatal_memerr("Allocation fails at CMDEN");
	if (!access(buf->txt, F_OK | X_OK) && !stat(buf->txt, &st) \
	&& (st.st_mode & S_IFREG))
		fpath = ft_memdup(buf->txt, buf->strlen + 1);
	dstr_del(&buf);
	return (fpath);
}

void		cmden_tree_pathentry(t_avl_tree *avl, char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	t_avln			*node;
	char			*fpath;

	if (access(path, F_OK) || !(dir = opendir(path)))
		return ;
	while ((entry = readdir(dir)) != NULL)
	{
		if (!(fpath = check_binary_file(path, entry->d_name)))
			continue ;
		if (!(node = ft_avl_node_cc(entry->d_name, fpath, CMDEN_BIN)))
			sys_fatal_memerr("Allocation fails at CMDEN");
		ft_avl_set(avl, node);
	}
	closedir(dir);
}

void		cmden_tree_builtn(t_avl_tree *avl, t_avln *bltns)
{
	t_avln	*node;
	char	*name;
	char	*path;

	if (!bltns || !avl)
		return ;
	if ((name = ft_strdup(bltns->key)))
	{
		if (!(path = ft_strdup("")))
			sys_fatal_memerr("Allocation fails at CMDEN");
		if ((node = ft_avl_node_cc(name, path, CMDEN_BLT)))
			ft_avl_set(avl, node);
		free(name);
	}
	cmden_tree_builtn(avl, bltns->left);
	cmden_tree_builtn(avl, bltns->right);
}

void		cmden_tree_funcs(t_avl_tree *avl, t_avln *bltns)
{
	t_avln	*node;
	char	*name;
	char	*path;

	if (!bltns || !avl)
		return ;
	if ((name = ft_strdup(bltns->key)))
	{
		if (!(path = ft_strdup("")))
			sys_fatal_memerr("Allocation fails at CMDEN");
		if ((node = ft_avl_node_cc(name, path, CMDEN_FUNC)))
			ft_avl_set(avl, node);
		free(name);
	}
	cmden_tree_funcs(avl, bltns->left);
	cmden_tree_funcs(avl, bltns->right);
}

t_avl_tree	*cmden_tree_create(ENV *env)
{
	t_avl_tree		*avl;
	size_t			i;
	char			**pths;
	t_dyn_string	*env_path;

	if (!(avl = ft_avl_tree_create(free_safe)))
		sys_fatal_memerr("Allocation fails at CMDEN");
	env_path = env_get_variable("PATH", env);
	if (env_path->strlen)
	{
		if (!(pths = ft_strsplit(env_path->txt, ':')))
			sys_fatal_memerr("Allocation fails at CMDEN");
		i = -1;
		while (pths[++i])
			;
		while (i-- > 0)
			cmden_tree_pathentry(avl, pths[i]);
		et_rm_warr(pths);
	}
	env_path ? dstr_del(&env_path) : 0;
	cmden_tree_builtn(avl, env->builtns->root);
	cmden_tree_funcs(avl, env->funcs->root);
	return (avl);
}
