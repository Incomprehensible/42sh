/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_options.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 11:18:22 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/28 13:51:04 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_OPTIONS_H
# define SH_OPTIONS_H

# include "ft_dlist.h"

/*
** 42sh IN FLAGS
**
** 42sh [-lib f1 f2 f3 --] \
** [-o opt=var/opt --] file param1 param2 param3
**
** -l <file1> <file2> <file3> -- - specify lib-files
** -o OPTION=NAME OPTION=NAME -- - options
**		OPTION:: color
*/

# define OPT_FSM_STT_LIM 7

typedef struct	s_arg_opt
{
	t_dlist		*lib_fs;
	t_dlist		*ops;
	t_dlist		*params;
}				t_opt;

typedef enum	e_fsm_stts
{
	stt_null = -1,
	stt_fin = 0,
	stt_select = 1,
	stt_ops = 2,
	stt_lb = 3,
	stt_fl = 4,
	stt_prm = 5,
	stt_err = 6
}				t_stts;

typedef int		t_state;
typedef t_state	(*t_stt_handler)(int *, char **, t_opt *);

int				opt_is_flend(const char *arg);
int				opt_is_opsfl(const char *arg);
int				opt_is_libfl(const char *arg);
int				opt_is_null(const char *arg);

t_state			stt_hnd_fin(int *ac, char **av, t_opt *opt);
t_state			stt_hnd_err(int *ac, char **av, t_opt *opt);
t_state			stt_hnd_select(int *ac, char **av, t_opt *opt);
t_state			stt_hnd_ops(int *ac, char **av, t_opt *opt);
t_state			stt_hnd_lib(int *ac, char **av, t_opt *opt);
t_state			stt_hnd_prm(int *ac, char **av, t_opt *opt);

int				opt_init(int ac, char **av, t_opt *opt);
void			et_rm_clear_opt(t_opt *opt);

#endif
