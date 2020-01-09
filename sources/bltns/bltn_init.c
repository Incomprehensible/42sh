/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 18:10:52 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/19 15:25:03 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_cnt_builtins(void)
{
	return (sizeof(g_bltns_str) / sizeof(char *));
}

int			bltn_init(ENV *env)
{
	int		i;
	char	path[256];

	i = -1;
	while (++i < bltn_cnt_builtins())
	{
		if ((ft_avl_set(env->builtns, ft_avl_node_cc(g_bltns_str[i],\
		g_bltns_func[i], sizeof(g_bltns_func[i])))) == -1)
			return (-1);
	}
	if (getcwd(path, sizeof(path)) == NULL)
		return (-1);
	g_pwd = dstr_new(path);
	g_oldpwd = dstr_new(path);
	return (1);
}
