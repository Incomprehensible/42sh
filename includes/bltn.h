/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:42:17 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/06 22:55:20 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLTN_H
# define BLTN_H
# include "libft.h"
# include "ft_avl_tree.h"
# include "env.h"
# include "rms.h"
# include "sys_tools/sys_tools.h"
# include <unistd.h>

static char	*g_bltns_str[] = {"setenv", "env", "getenv", "unsetenv", "exit",\
	"cd", "pwd", "type", "((", "echo", "fg", "bg", "jobs",\
	"export", "unset", "set", "source"};

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
** FG_BG_HELPERS
*/

ssize_t				fg_args_get_jid(char *job);
ssize_t				fg_by_str(char *job);
int					bltn_fg_bg_launch(ssize_t a, int mode);

/*
** JOBS_HELPRES
*/

int					jobs_put_by_mode(const ssize_t i, int md);
int					jobs_put_long_entry(const ssize_t i);
int					jobs_put_pids(const ssize_t i);
int					jobs_put_entry_sm(const ssize_t i);

int					no_job_error(char *job, char *cont, ENV *envr);
char				*jobs_state_str_set(const t_pgrp *pg);
char				jobs_cur_char_set(ssize_t i);
int					ft_isodigit(int ch);

int					bltn_math(char **args, ENV *env);

int					bltn_set(char **argv, ENV *env);

# define CD_MODE_ERROR_ND -3
# define CD_MODE_ERROR_PD -2
# define CD_MODE_ERROR_NE -1
# define CD_MODE_ERROR 0
# define CD_MODE_LOGIC 1
# define CD_MODE_PHYSP 2
# define CD_MODE_TOOMA -1

int					bltn_cd(char **args, ENV *env);
int					cd_logic(DSTRING **path);
int					cd_phpath(DSTRING **path);
int					cd_parse_args(char **args, ENV *env, DSTRING **path);

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
int					bltn_source(char **args, ENV *envr);

void				et_rm_str(void *cont, size_t ske);

static int	(*g_bltns_func[]) (char **, ENV *) = {&bltn_setenv,\
	&bltn_env, &bltn_getenv, &bltn_unsetenv, &bltn_exit,\
	&bltn_cd, &bltn_pwd, &bltn_type,\
	&bltn_math, &bltn_echo, &bltn_fg, &bltn_bg,\
	&bltn_jobs, &bltn_export, &bltn_unset, &bltn_set, &bltn_source};

#endif
