/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_vars_gets.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 19:18:21 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/15 19:32:33 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include <dirent.h>

static void	sh_vars_parsepath(t_darr **res, char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**spl;
	int				i;

	i = -1;	
	spl = ft_strsplit(path, ':');
	while (*spl)
	{
		dir = opendir(*spl);
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name[0] != '.')
			{
				(*res)->strings[++i] = dstr_new((char *)entry->d_name);
				(*res)->count++;
			}
			if (entry->d_namlen > (*res)->maxlen)
				(*res)->maxlen = (size_t)entry->d_namlen;
			(*res)->allsize += (size_t)entry->d_namlen;
		}
		closedir(dir);
		spl++;
	}
	(*res)->strings[++i] = NULL;
}

t_darr		*get_list_cmds(t_envp	*envp)
{
	t_darr	*res;
	
	if(!(res = (t_darr*)malloc(sizeof(t_darr))))
		exit (1);
	res->allsize = 0;
	res->maxlen = 0;
	res->maxlen = 0;
	res->count = 0;
	if (ft_avl_search(envp->global, "PATH") != 0)
		sh_vars_parsepath(&res, (char *)ft_avl_search(envp->global, "PATH")->content);	
	// else if (ft_avl_search(envp->local, "PATH") != 0)
	// 	sh_vars_parsepath(&res, (char *)ft_avl_search(envp->local, "PATH")->content);	 
	else
		return (NULL);
	return (res);
}