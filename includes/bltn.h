/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:42:17 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/28 19:25:18 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLTN_H
# define BLTN_H

# include "libft.h"
# include "ft_avl_tree.h"
# include "env.h"
# include "rms.h"

static char *bltns_str[] = {"setenv", "env", "getenv", "unsetenv"};

/* Волзвращает кол-во bltns */
int	bltn_cnt_builtins();

/* Инициализация bltns*/
int	bltn_init(ENV *env);

/* Добавляет новую глобальную переменную или изменяет уже существующую с ключом $key и значением $value. */
int	bltn_setenv(char **args, ENV *env);

/* Выводит в стандартный вывод значение всех глобальных переменных в формате ключ=значение. */
int	bltn_env(char **argc, ENV *env);

/* Выводит в стандартный вывод значение глобальной переменной с ключом*/
int	bltn_getenv(char **args, ENV *env);

/* Удаляет глобальную переменную.*/
int	bltn_unsetenv(char **args, ENV *env);

static int (*bltns_func[]) (char **, ENV *) = {&bltn_setenv, &bltn_env, &bltn_getenv, &bltn_unsetenv};

#endif