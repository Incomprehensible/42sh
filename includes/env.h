/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 15:25:05 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/24 16:08:53 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "ft_avl_tree.h"

# define ENV t_env

typedef struct	s_env
{
	t_avl_tree	*locals;
	t_avl_tree	*globals;
	t_avl_tree	*builtns;
	t_avl_tree	*funcs;
}				t_env;

#endif
