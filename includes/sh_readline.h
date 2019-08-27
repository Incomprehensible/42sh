/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:53:02 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/27 03:09:54 by gdaemoni         ###   ########.fr       */
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

#define S_ASTR_STR 50000

/* 
** ch - user entered character
** ind - where to insert the character
*/
typedef struct	s_indch
{
	char		ch;
	size_t		ind;
}				t_indch;

/* array of strings with a counter */
typedef struct	s_astr
{
	DSTRING		*strings[S_ASTR_STR];
	size_t		count;
}				t_astr;

/* if flag 1 means the path is part of the buffer */
typedef struct	s_regpath
{
	DSTRING		*path;
	char		fl;
}				t_regpath;

typedef struct	s_fl
{
	char		tab;
	char		astr;
}				t_fl;

DSTRING			*sh_readline(t_envp *env);

/* INIT GLOBAL AND LOCAL ENV
	argc - count of arguments from main()
	argv - arguments from main()
	envp - envp from main()
	env  - adreess for env */
void			init_vars(int argc, char **argv, char **envp, t_envp *env);

/*	GET ALL CMDS FROM $PATH */
t_darr			get_list_cmds(t_envp *envp);

/* overwrites the buffer string in the console and sets the cursor at the index */
void			sh_rewrite(const DSTRING *buf, const size_t index);

/* deletes a character by index. flag == BAKSP moves carriage one character back */
ssize_t			sh_del_char(DSTRING **buf, size_t index, const char flag);

/* checks if the user enters a command or directory */
ssize_t			sh_dstr_iscmd(const DSTRING *str);

/* search for a character from the end of a line. returns character index or -1 */
ssize_t			dstrrchr(const DSTRING *src, const int ch);

/* removes a char on an index */
ssize_t			dstr_del_char(DSTRING **src, ssize_t n);

/*
** changes dst:
** cuts out from dst everything after the index inclusively. inserts at the end of src
*/
void			dstr_cutins_dstr(DSTRING **dst, DSTRING *src, size_t ind);
void			dstr_cutins_str(DSTRING **dst, char *src, ssize_t ind);
void			dstr_cutins_ch(DSTRING **dst, char ch, ssize_t ind);

/* puts the contents of the directory in t_darr */
t_darr			sh_dir_content(char *path);

/* checks the buffer slice from the index if the directory is written there return 1 */
char			sh_isdir(DSTRING *buf, ssize_t start_dir);

/* will return 0 if the path is "." or ".." else 1 */
char			sh_check_dot(const DSTRING *path);

/* clears an array DSTRING */
void			free_darr(DSTRING **darr);
void			free_darr_n(DSTRING **darr, const size_t size);

/* sorts lexicographically array DSTRING */
int				sort_darr(t_darr *darr);

/* handles arrow clicks */
size_t			sh_esc(size_t index, const size_t max);

/* intercepts all keystrokes on the keyboard */
int				ft_getch(void);

/* prints an array in the form of columns. does not move the carriage */
void			put_col(t_darr darr);

/* auto completion */
t_indch			sh_tab(DSTRING **buf, size_t index, t_envp *env);
t_darr			sh_add_cmd(DSTRING **buf, t_envp *env);
t_darr			sh_add_path(DSTRING **buf, size_t start_dir);
char			sh_check_back_slash(DSTRING **buf, const ssize_t start_dir);

/* appends directory name */
void			correct_namedir_buf(t_darr darr, DSTRING **buf, size_t start_dir);

/* returns a t_darr of matches with str */
t_darr			sh_cmp_darr(const t_darr darr, DSTRING *str);

/* returns the path written in the buffer by index or "." */
DSTRING			*sh_get_path(DSTRING *buf, size_t start_dir);

/*
** concatenates strings into one. if the special line “00” is connected between the lines, 
** no separator will be inserted, if the special line “1 /” between the lines /
*/
char			*ft_concat(const size_t n, const char *spec, ...);

/* handles a special asterisk character */
int				astr(DSTRING **buf, t_fl *fl);

/* auto-replace astr */
void			loop(DSTRING *reg, int i, t_astr *rez, const int itr);

/* help astr */
char			chek_astr(const DSTRING *reg);
DSTRING			*add_slash(char *str, DSTRING *reg);
char			is_strdot(const char *path);
DSTRING			*join_reg(DSTRING *n_dir, DSTRING *cmp, const char fl);
char			cmp_dirreg(DSTRING *n_dir, DSTRING *cmp,\
						 DSTRING *reg, const char fl);
DSTRING			*slice_reg(DSTRING *reg);
int				nmatch(const char *s1, const char *s2);

/* returns regular expression */
DSTRING			*cut_reg_expr(DSTRING *buf);

/*
** parsing a regular expression if there is a path in it,
** returns the path, if there is no path, returns "."
*/
t_regpath		get_regpath(DSTRING *reg);

#endif
