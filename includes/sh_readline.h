/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:53:02 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/04 17:00:23 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_READLINE_H
# define SH_READLINE_H

# include <termcap.h>
# include <stdio.h>
# include <dirent.h>
# include "dstring.h"
# include "ft_avl_tree.h"
# include "libft.h"
# include "env.h"

#define S_ASTR_STR 50000

char			*tmp_readline(char *prompt);

/* to save strings ft_concat */
typedef struct	s_concat
{
	size_t		start;
	size_t		*size;
	size_t		n;
	size_t		i;
	size_t		all;

}				t_concat;

/* 
** ch - user entered character
** ind - where to insert the character
** fl - 0 вызываем гетчар. 1 не вызываем
** reg = 1 если регулярное выражение
** tab = 1 если нажат таб
*/
typedef struct	s_indch
{
	char		ch;
	int			ind;
	char		fl;
	int			his;
	int			reg;
	int			tab;
	DSTRING		*prompt;
}				t_indch;

/* to save strings sh_tab */
typedef struct	s_name_ind
{
	int			ind;
	int			ind_name;
	t_indch		indch;
}				t_name_ind;

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
	char		reg;
}				t_fl;

size_t			prebuf;
int				preind;
t_darr			g_histr;

void			init_histr(ENV *envr);
t_indch			show_new_history(DSTRING **buf, t_indch indc, ENV *envr);

DSTRING			*sh_new_redline(const DSTRING *prompt, ENV *env);


t_indch			management_line(t_indch indch, DSTRING **buf, ENV *envr);
char			is_reg(DSTRING *buf);
void			fill_buf(DSTRING **buf, const t_astr rez);
int				sh_move_insertion_point(const DSTRING *str, int ind, const char ch);


DSTRING			*dstr_nerr(char *src);
DSTRING			*dstr_serr(DSTRING *src, ssize_t bi, ssize_t ei);
DSTRING			*dstr_scerr(DSTRING **src, ssize_t bi, ssize_t ei);
DSTRING			*check_null(DSTRING *line);

char			is_ctrl(const t_indch indch);

/* 
** Command line editing
**
** ctrl+a move the cursor to the beginning of the line
** ctrl+e move the cursor to the end of the line
** ctrl+u Delete to the beginning of the line
** ctrl+t Delete to the end of the line
** ctrl+x move insertion point one word back
** ctrl+f move insertion point one word ahead
** ctrl+n clear all screen
*/
t_indch			management_line(t_indch indch, DSTRING **buf, ENV *envr);
void			clear_screen(void);
int				sh_t_insertion_point(const DSTRING *str, int ind, const char ch);

/*	GET ALL CMDS FROM $PATH */
t_darr			get_list_cmds(ENV *envp);

/* overwrites the buffer string in the console and sets the cursor at the index */
void			sh_rewrite(const DSTRING *buf, const size_t index);

void			sh_new_rewrite(const DSTRING *prompt, const DSTRING *buf,\
						 const size_t index);

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
void			free_t_darr(t_darr *darr);
void			free_darr_n(DSTRING **darr, const int size);
void			free_darr_re(DSTRING **darr, const int size);

/* sorts lexicographically array DSTRING */
int				sort_darr(t_darr *darr);

/* handles arrow clicks */
t_indch			sh_esc(t_indch indch, const size_t max, DSTRING **buf, ENV *env);

/* intercepts all keystrokes on the keyboard */
int				ft_getch(void);

/* prints an array in the form of columns. does not move the carriage */
void			put_col(t_darr overlap, const DSTRING *buf, t_indch indch);
ushort			get_col(const int lencol);
DSTRING			*sh_get_col(t_darr dar, const ushort col, ushort iter);
void			free_lines_down(void);

/* auto completion */
t_indch			sh_tab(DSTRING **buf, ENV *env, t_indch indch);
t_darr			sh_tab_help(DSTRING **buf, ENV *env, t_indch indch);
int				sh_tab_loop_help(t_darr overlap, DSTRING **buf, \
					int fl, t_name_ind n_ind);
t_darr			sh_add_cmd(DSTRING **buf, ENV *env);
t_darr			sh_add_path(DSTRING **buf, size_t start_dir);
char			sh_check_back_slash(DSTRING **buf, const ssize_t start_dir);
int				ind_begin_cmd(DSTRING *buf);
void			subst_name(DSTRING **buf, t_darr overlap,\
					int ind, t_name_ind n_ind);
char			sh_check_back_slash(DSTRING **buf, const ssize_t start_dir);

/* appends directory name */
void			correct_namedir_buf(t_darr darr, DSTRING **buf, size_t start_dir);
void			help_correct_namedir_buf(t_darr *darrcopy, DSTRING **name_fil,\
					DSTRING **name_dir);

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
int				reg_expr(DSTRING **buf, t_fl *fl);

/* auto-replace reg_expr */
void			loop(DSTRING *reg, int i, t_astr *rez, const int itr);

/* help reg_expr */
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


/*
** To enable history management, click the up or down arrow
**
** ctrl+l clear history buf
** ctrl+k print columns in chronological order
** ctrl+p print sorted column
** ctrl+r search history
** tab	  show overlap history
*/

int				get_history_fd(int flags, char *er_context, ENV *envr);

/* write history in file */
void			write_history(DSTRING *line);

/* history management */
t_indch			show_history(DSTRING **buf, t_indch indc, ENV *envr);

t_indch			sh_new_search_h(DSTRING **buf, ENV *envr, t_indch indch);

void			direction_help(t_darr o, t_darr his, t_indch *ich,\
					DSTRING **strd);

#define LENSERCH 20

/* overwrites the command history file to avoid buffer overflow */
void			rewrite_histr(t_darr *histr, ENV *envr);

/* reads history file and fill struct t_darr */
char			get_histr(t_darr *histr, ENV *envr);

/* reads pressed keys */
char			ispers_arws(char ch, t_indch *indch, \
				t_darr *his, const DSTRING *buf);

void			sys_term_init(void);
void			sys_term_restore(void);
void			sys_term_noecho(void);

#endif
