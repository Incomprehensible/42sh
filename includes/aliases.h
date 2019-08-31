/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 14:39:09 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/31 17:02:18 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIASES_H
# define ALIASES_H
# include "ft_avl_tree.h"
# include "libft.h"
# include "env.h"
# include "bltn.h"
# include <unistd.h>
# include <fcntl.h>
# define FILE_ALIASES "/Users/fnancy/.42shrc"

/*
**Инициализация алиасов
*/
int		alias_init(ENV *env);
/*
**Добавление алиаса в файл конфига
*/
int		alias_add(ENV *env);

#endif
