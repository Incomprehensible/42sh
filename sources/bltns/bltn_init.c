/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 18:10:52 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/18 22:10:42 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_cnt_builtins(void)
{
	return (sizeof(bltns_str) / sizeof(char *));
}

int			bltn_init(ENV *env)
{
	int		i;
	char	path[256];

	i = -1;
	while (++i < bltn_cnt_builtins())
	{
		if ((ft_avl_set(env->builtns, ft_avl_node_cc(bltns_str[i],\
		bltns_func[i], sizeof(bltns_func[i])))) == -1)
			return (-1);
	}
	if (getcwd(path, sizeof(path)) == NULL)
		return (-1);
	pwd = dstr_new(path);
	oldpwd = dstr_new(path);
	return (1);
}
