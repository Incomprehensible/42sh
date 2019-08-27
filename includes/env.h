/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 15:25:05 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/27 16:40:10 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "ft_avl_tree.h"
# include "libft.h"
# include "dstring.h"

# define ENV t_env

typedef struct	s_env
{
	t_avl_tree	*locals;
	t_avl_tree	*globals;
	t_avl_tree	*builtns;
	t_avl_tree	*funcs;
}				t_env;

#endif

/* INIT ENV 
 -if success return 1
 -else (errcode)
 */
	
int		env_init(int argc, char **argv, char **envp, ENV *env);

/* Возвращает дубликат значения перменной среды */
DSTRING	*env_get_variable(const char *key, ENV *enviroment);

/* Добавляет или изменяет существующую переменную в среду. */
int		env_set_variable(const char *key, DSTRING *value, ENV *enviroment);

/* Удаляет переменную и очищает её память.*/
int		env_unset_variable(const char *key, ENV *enviroment);
