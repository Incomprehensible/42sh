/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_core_setter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 21:54:42 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/20 21:59:47 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int			env_core_set(char *key, char *value, t_avl_tree *core)
{
	t_avln	*node;

	if (!(node = ft_avl_node(key, value, ft_strlen(value) + 1)))
		return (-1);
	if ((ft_avl_set(core, node)) < 0)
		return (-1);
	return (0);
}

int			prs_set_last_status(int *status, ENV *envr)
{
	char	*val;

	val = ft_itoa(*status);
	env_core_set("?", val, envr->core);
	free(val);
	return (0);
}
