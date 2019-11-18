/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 15:25:05 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/18 22:58:42 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "ft_avl_tree.h"
# include "libft.h"
# include "dstring.h"

# define S_DARR_STRINGS		2096
# define ENV t_env

typedef struct	s_env
{
	t_avl_tree	*locals;
	t_avl_tree	*globals;
	t_avl_tree	*builtns;
	t_avl_tree	*funcs;
	t_avl_tree	*aliases;
	t_avl_tree	*core;
}				t_env;

/*	strings - array of DSTRING'S(every string is name of command / directory)
	maxlen  - max length name command / directory
	count   - count elements in array
	allsize - count symbols all elements in array */
typedef struct	s_darr
{
	DSTRING		*strings[S_DARR_STRINGS];
	size_t 		count;
	size_t 		maxlen;
	size_t 		allsize;
}				t_darr;

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
int		env_unset_func(const char *key, ENV *enviroment);

/*Функция для обработки ошибок инициализации */
int		env_init_error(ENV *env, int errcode);

void	free_spl(char ***spl);

int		env_core_set(char *key, char *value, t_avl_tree *core);
int		prs_set_pid(ENV *envr);

void	env_get_bins_parsepath(t_darr *res, ENV *envp);
void	env_get_bins_parsebltn(t_darr *res, ENV *envp);
void	env_get_bins_parefunc(t_darr *res, ENV *envp);

t_darr	env_get_bins(ENV *env);

int		env_get_bins_unq(t_darr *res, char *str);
void	env_get_bins_cmp_name(t_darr *res, int namelen);
t_darr	env_get_keys(ENV *envp, char *pref);

#endif

