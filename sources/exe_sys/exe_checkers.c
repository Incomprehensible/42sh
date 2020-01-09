/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_checkers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 20:52:30 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/16 20:52:55 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_avl_tree.h"
#include "ft_string.h"
#include "parser.h"
#include "sys_tools/sys_tools.h"
#include "bltn_hash.h"

char		*cache_check(const char *cmd, ENV *env, pid_t *pid)
{
	t_hentr		*entry;

	entry = ht_get(env->htid, (char *const)cmd);
	if (!entry)
		return (NULL);
	entry->hits += 1;
	*pid = set_pid_err(*pid, entry->path);
	if (!(*pid))
		return (ft_strdup(entry->path));
	else
		return (0);
}

char		*tree_check(const char *cmd, ENV *env, pid_t *pid)
{
	t_avln		*node;

	if (!(node = ft_avl_search(env->cmds, cmd)))
		return (NULL);
	*pid = set_pid_err(*pid, node->content);
	if (!(*pid))
		return (ft_strdup(node->content));
	else
		return (0);
}

char		*path_check(const char *cmd, ENV *env, pid_t *pid)
{
	t_hentr			entry;
	char			*key;

	*pid = set_pid_err(*pid, (char *)cmd);
	if (!(*pid))
	{
		if (!(key = ft_basename(cmd)))
			sys_fatal_memerr("ERROR AT CACHING A PATH");
		realpath(cmd, entry.path);
		entry.hits = 1;
		if ((ht_add(env->htid, key, &entry, sizeof(t_hentr))) == HT_ALLOCAFAIL)
			sys_fatal_memerr("ERROR AT CACHING A PATH");
		free(key);
		return (ft_strdup(cmd));
	}
	else
		return (0);
}
