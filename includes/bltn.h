/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:42:17 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/19 15:23:44 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLTN_H
# define BLTN_H
# include "libft.h"
# include "ft_avl_tree.h"
# include "env.h"
# include "rms.h"
# include <unistd.h>

static char	*g_bltns_str[] = {"setenv", "env", "getenv", "unsetenv", "exit",\
	"alias", "unalias", "cd", "pwd", "type", "((", "echo", "fg", "bg", "jobs",\
	"export", "unset", "set"};

t_dyn_string		*g_pwd;
t_dyn_string		*g_oldpwd;

typedef struct		s_flags
{
	char			first;
	char			second;
}					t_flags;

char				*ft_parse_flags(char **argc);
size_t				args_len(char **argv);
int					no_job_error(char *job, char *cont, ENV *envr);

/*
**Возвращает кол-во bltns
*/
int					bltn_cnt_builtins();

/*
**Инициализация bltns
*/
int					bltn_init(ENV *env);

/*
**Добавляет новую глобальную переменную или изменяет уже существующую с ключом
**$key и значением $value.
*/
int					bltn_setenv(char **args, ENV *env);

/*
**Выводит в стандартный вывод значение всех глобальных переменных в формате
**ключ=значение.
*/
int					bltn_env(char **argc, ENV *env);

/*
**Выводит в стандартный вывод значение глобальной переменной с ключом
*/
int					bltn_getenv(char **args, ENV *env);

/*
**Удаляет глобальную переменную.
*/
int					bltn_unsetenv(char **args, ENV *env);

/*
**Завершение программы
*/
int					bltn_exit(char **args, ENV *env);

/*
**Создать алиас. Если нет аргументов, то вывести список всех алиасов
*/
int					bltn_alias(char **args, ENV *env);

/*
**Удаление алиаса
*/
int					bltn_unalias(char **args, ENV *env);

/*
**Математический билтын
*/
int					bltn_math(char **args, ENV *env);

int					bltn_set(char **argv, ENV *env);
int					bltn_cd(char **args, ENV *env);
int					bltn_cd_parser(ENV *env, t_dlist **path);
DSTRING				*bltn_cd_pathtostr(t_dlist *path);
void				bltn_cd_destroy_path(t_dlist **path);
void				bltn_cd_freepaths(DSTRING **oldpath, \
						DSTRING **newpath, t_dlist	**path);
int					bltn_cd_error(DSTRING **oldpath, \
						DSTRING **newpath, t_dlist	**path);
void				bltn_cd_concat(char **path);
int					bltn_cd_countargs(char **args);
int					bltn_pwd(char **args, ENV *env);
int					bltn_type(char **args, ENV *env);
int					bltn_echo(char **args, ENV *envr);
int					bltn_fg(char **args, ENV *envr);
int					bltn_bg(char **args, ENV *envr);
int					bltn_jobs(char **args, ENV *envr);
int					bltn_export(char **args, ENV *envr);
int					bltn_unset(char **args, ENV *envr);

void				et_rm_str(void *cont, size_t ske);

static int	(*g_bltns_func[]) (char **, ENV *) = {&bltn_setenv,\
	&bltn_env, &bltn_getenv, &bltn_unsetenv, &bltn_exit,\
	&bltn_alias, &bltn_unalias, &bltn_cd, &bltn_pwd, &bltn_type,\
	&bltn_math, &bltn_echo, &bltn_fg, &bltn_bg,\
	&bltn_jobs, &bltn_export, &bltn_unset, &bltn_set};

#endif
