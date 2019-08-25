/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:53:02 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/25 09:17:35 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_READLINE_H
# define SH_READLINE_H

# include <termcap.h>
# include <stdio.h>
# include "dstring.h"
# include "ft_avl_tree.h"
# include "libft.h"
# include "sh_vars.h"

# define S_DARR_STRINGS		2096

typedef struct s_indch
{
	char	ch;
	size_t	ind;
}				t_indch;


/* INIT GLOBAL AND LOCAL ENV
	argc - count of arguments from main()
	argv - arguments from main()
	envp - envp from main()
	env  - adreess for env*/
void			init_vars(int argc, char **argv, char **envp, t_envp *env);

/*	GET ALL CMDS FROM $PATH */
t_darr			get_list_cmds(t_envp *envp);

char			*sh_readline(t_envp *env);
void			sh_rewrite(const DSTRING *buf, const size_t index);


ssize_t			sh_del_char(DSTRING **buf, size_t index, const char flag);
ssize_t			dstr_del_char(DSTRING **src, ssize_t n);
ssize_t			sh_dstr_iscmd(const DSTRING *str);
ssize_t			dstrrchr(const DSTRING *src, const int ch);

/*
** free dst; return newdst
*/
void			dstr_cutins_dstr(DSTRING **dst, DSTRING *src, size_t ind);
void			dstr_cutins_str(DSTRING **dst, char *src, ssize_t ind);
void			dstr_cutins_ch(DSTRING **dst, char ch, ssize_t ind);

void			dstr_cutins_dstr(DSTRING **dst, DSTRING *src, size_t ind);

t_darr			sh_dir_content(char *path);
char			sh_isdir(DSTRING *buf, ssize_t start_dir);
char			sh_check_dot(const DSTRING *path);

void			free_darr(DSTRING **darr);
void			free_darr_n(DSTRING **darr, const size_t size);
int				sort_darr(t_darr *darr);

size_t			sh_esc(size_t index, const size_t max);
int				ft_getch(void);

void			put_col(t_darr darr);

t_indch			sh_tab(DSTRING **buf, size_t index, t_envp *env);
t_darr			sh_add_cmd(DSTRING **buf, t_envp *env);
t_darr			sh_add_path(DSTRING **buf, size_t start_dir);
char			sh_check_back_slash(DSTRING **buf, const ssize_t start_dir);
int				sort_darr(t_darr *darr);

t_darr			sh_cmp_darr(const t_darr darr, DSTRING *str);
void			correct_namedir_buf(t_darr darr, DSTRING **buf, size_t start_dir);

DSTRING			*sh_get_path(DSTRING *buf, size_t start_dir);
char			sh_isdir(DSTRING *buf, ssize_t start_dir);

char			*ft_concat(const size_t n, const char *spec, ...);


int			astr(DSTRING **buf);

#endif
