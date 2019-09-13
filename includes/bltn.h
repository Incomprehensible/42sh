/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:42:17 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/13 06:25:17 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLTN_H
# define BLTN_H
# include "libft.h"
# include "ft_avl_tree.h"
# include "env.h"
# include "rms.h"
# include <unistd.h>

static char	*bltns_str[] = {"setenv", "env", "getenv", "unsetenv", "exit",\
	"alias", "unalias", "cd", "pwd"};

char		*ft_parse_flags(char **argc);

/*
**Волзвращает кол-во bltns
*/
int	bltn_cnt_builtins();

/*
**Инициализация bltns
*/
int	bltn_init(ENV *env);

/*
Добавляет новую глобальную переменную или изменяет уже существующую с ключом $key и значением $value.
*/
int	bltn_setenv(char **args, ENV *env);

/*
Выводит в стандартный вывод значение всех глобальных переменных в формате ключ=значение.
*/
int	bltn_env(char **argc, ENV *env);

/*
Выводит в стандартный вывод значение глобальной переменной с ключом
*/
int	bltn_getenv(char **args, ENV *env);

/*
Удаляет глобальную переменную.
*/
int	bltn_unsetenv(char **args, ENV *env);

/*
Завершение программы
*/
int bltn_exit(char **args, ENV *env);

/*
**Создать алиас. Если нет аргументов, то вывести список всех алиасов
*/
int	bltn_alias(char **args, ENV *env);

/* Удаление алиаса*/
int	bltn_unalias(char **args, ENV *env);

int	bltn_cd(char **args, ENV *env);

int	bltn_pwd(char **args, ENV *env);

static int	(*bltns_func[]) (char **, ENV *) = {&bltn_setenv,\
	&bltn_env, &bltn_getenv, &bltn_unsetenv, &bltn_exit, &bltn_alias, &bltn_unalias,\
	&bltn_cd, &bltn_pwd};

#endif
