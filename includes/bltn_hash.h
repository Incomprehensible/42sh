/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_hash.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 13:38:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/16 21:02:01 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLTN_HASH_H
# define BLTN_HASH_H

# include <limits.h>
# include <stdlib.h>
# include "env.h"

typedef struct	s_hs_entry
{
	char		path[PATH_MAX];
	size_t		hits;
}				t_hentr;

typedef struct	s_bltn_ops_hs
{
	char		path[1024];
	char		**args;
	int			rest;
	char		*err;
}				t_bltn_ohs;

# define CACHE_STD_SIZE 64

char			*cache_check(const char *cmd, ENV *env, pid_t *pid);
char			*tree_check(const char *cmd, ENV *env, pid_t *pid);
char			*path_check(const char *cmd, ENV *env, pid_t *pid);
pid_t			set_pid_err(pid_t pid, char *path);
int				hsblt_printhashes(t_bltn_ohs *os, ENV *env);
int				hsblt_put_all(t_ht_id *htid);
int				hsblt_put_entry(char *key, t_hentr *entry, int frst);
int				hsblt_reset(t_bltn_ohs *os, ENV *env);
int				hsprs_errhandler(char *mess, int ret_code);
int				hsblt_sethashes(t_bltn_ohs *os, ENV *env);
t_bltn_ohs		hash_parse_args(char **args);
int				hprs_no_entry_err(char *entry, char *message);
int				hprs_errors(int state, t_bltn_ohs *ops);
int				hprs_flags(t_bltn_ohs *ops, size_t *i, char **args);
int				hprs_args(t_bltn_ohs *ops, size_t *i, char **args);

#endif
