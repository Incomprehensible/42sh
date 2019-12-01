/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:53:02 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/12/01 14:33:43 by hgranule         ###   ########.fr       */
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
# include <unistd.h>
# include <sys/ioctl.h>
# include "sh_termcomand.h"
# define S_ASTR_STR 50000
# define SPEC_SYMBOLS " $=&|)('\"><{}*~`\\"

char			*tmp_readline(char *prompt);

/*
** to save strings ft_concat
*/
typedef struct	s_concat
{
	size_t		start;
	size_t		*size;
	size_t		n;
	size_t		i;
	size_t		all;
}				t_concat;

typedef struct	s_clip_board
{
	DSTRING		*buffer;
	int			ind;
}				t_clipbrd;


typedef struct	s_indch
{
	DSTRING		*prompt;
	int			ind;
	// int			his;
	int			reg;
	int			var;
	int			is_var;
	int			tab;
	int			fl_t;
	int			type_inp;
	int			ind_inp;
	int			ind_slash;
	int			select;
	int			exit;
	char		fl;
	char		ch;
}				t_indch;

typedef struct	s_buf
{
	DSTRING		*begin;
	DSTRING		*end;
	DSTRING		*buf;
	DSTRING		*val;
	DSTRING		*sub;
	DSTRING		*dir;
	int			cut;
	int			slash;
}				t_buf;

/*
** to save strings sh_ta
*/
typedef struct	s_name_ind
{
	int			ind;
	int			fl;
	int			road;
	int			ind_name;
	t_indch		indch;
}				t_name_ind;

/*
** array of strings with a counter
*/
typedef struct	s_astr
{
	DSTRING		*strings[S_ASTR_STR];
	size_t		count;
}				t_astr;

/*
** if flag 1 means the path is part of the buffer
*/
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

typedef struct	s_beend
{
	int			begin;
	int			end;
}				t_beend;

size_t			g_prebuf;
int				g_preind;
t_darr			g_histr;

DSTRING			*sh_readline(const DSTRING *prompt, ENV *env);
void			fl_input(const DSTRING *buf, t_indch *indch);
void			return_line(DSTRING **buf, t_indch *indch, ENV *env);
int				reg_expr(DSTRING **buf, t_indch *indch, ENV *env);
t_buf			slicer_reg(DSTRING **buf);
void			sh_type_input(DSTRING *buf, t_indch *indch);
void			clip_work(t_indch *indch, t_clipbrd *clip, DSTRING **buf);
void			sh_tab(DSTRING **buf, t_indch *indch, ENV *env);
t_name_ind		tab_loop(t_darr overlap, t_buf *buffer, int fl, \
					t_name_ind n_ind);
t_darr			get_overlap(t_buf *buffer, t_indch *indch, ENV *env);
t_buf			slicer(DSTRING **buf, t_indch *indch, ENV *env);
void			unite_buf(DSTRING **buf, t_buf *buffer);
int				end_cut(char *str, int start, char ch);
void			dell_slash(DSTRING **sub);
int				is_var(t_buf *buffer, t_darr *allvar, ENV *env);
t_darr			get_executable_files(char *path);
int				is_executable(t_buf *bufffer);
DSTRING			*get_path(DSTRING *file, int fl);
void			check_spec_symbols(DSTRING **str);

char			is_sysdir(char *name, char *sub);
int				ft_isdir(DSTRING *buf);

void			init_histr(ENV *envr);
void			print_his_line(DSTRING *str_srch, DSTRING *str_over);
void			skip_escp(void);
void			fill_buf(DSTRING **buf, const t_astr rez);
int				sh_move_insertion_point(const DSTRING *str, \
					int ind, const char ch);

DSTRING			*dstr_nerr(char *src);
DSTRING			*dstr_serr(DSTRING *src, ssize_t bi, ssize_t ei);
DSTRING			*check_null(DSTRING *line);

char			is_ctrl(const char ch);
void			sh_move_up_lines(int lines);
void			sh_move_back(int move_back);
void			sh_clear_buf(struct winsize term, int len_p, int index);
void			clip_index(int x1, int x2, int *y1, int *y2);

/*
** Command line editing
** ctrl+a move the cursor to the beginning of the line
** ctrl+e move the cursor to the end of the line
** ctrl+u Delete to the beginning of the line
** ctrl+t Delete to the end of the line
** ctrl+x move insertion point one word back
** ctrl+f move insertion point one word ahead
** ctrl+n clear all screen
*/
t_indch			management_line(t_indch indch, DSTRING **buf, t_clipbrd *clip);
void			clear_screen(void);
int				sh_t_insertion_point(const DSTRING *str, \
						int ind, const char ch);

/*
** GET ALL CMDS FROM $PATH
*/
t_darr			get_list_cmds(ENV *envp);

/*
** overwrites the buffer string in the console and sets the cursor at the index
*/
void			sh_rewrite(const DSTRING *prompt, const DSTRING *buf,\
						const size_t index, int i);
void			get_coord_caret(int *x, int *y);
void			sh_putstr_term_c(DSTRING *buf, struct winsize term, \
					int *lensi);
int				skip_str(char *str);
/*
** deletes a character by index. flag == BAKSP moves carriage one character back
*/
int				sh_del_char(DSTRING **buf, int ind, const char cmd);

/*
** search for a character from the end of a line. returns character index or -1
*/
ssize_t			dstrrchr(const DSTRING *src, const int ch);

/*
** removes a char on an index
*/
ssize_t			dstr_del_char(DSTRING **src, ssize_t n);

/*
** changes dst:
** cuts out from dst everything after the index inclusively.
** inserts at the end of src
*/
void			dstr_cutins_dstr(DSTRING **dst, DSTRING *src, size_t ind);
void			dstr_cutins_str(DSTRING **dst, char *src, ssize_t ind);
void			dstr_cutins_ch(DSTRING **dst, char ch, ssize_t ind);

DSTRING			*dstr_scerr(DSTRING **src, ssize_t bi, ssize_t ei);
DSTRING			*dstr_serr(DSTRING *src, ssize_t bi, ssize_t ei);
DSTRING			*dstr_nerr(char *src);

/*
** puts the contents of the directory in t_darr
*/
t_darr			sh_dir_content(char *path, DSTRING *sub);

/*
** checks the buffer slice from the index if the directory
** is written there return 1
*/
char			sh_isdir(DSTRING *buf, ssize_t start_dir);

/*
** will return 0 if the path is "." or ".." else 1
*/
char			sh_check_dot(const DSTRING *path);

/*
** clears an array DSTRIN
*/
void			free_t_darr(t_darr *darr);
void			free_t_astr(t_astr *darr);
void			free_darr_re(DSTRING **darr, const int size);

/*
** sorts lexicographically array DSTRING
*/
int				sort_darr(t_darr *darr);

/*
** handles arrow clicks
*/
t_indch			sh_esc(t_indch indch, const size_t max, \
DSTRING **buf, t_clipbrd *clip);

/*
** intercepts all keystrokes on the keyboard
*/
int				ft_getch(void);

/*
** prints an array in the form of columns. does not move the carriage
*/
void			put_col(t_darr overlap, const DSTRING *buf, t_indch indch);
ushort			get_col(const int lencol);
DSTRING			*sh_get_col(t_darr dar, const ushort col, ushort iter);
void			free_lines_down(void);

/*
** appends directory name
*/
void			help_correct_namedir_buf(t_darr *darrcopy, DSTRING **name_fil,\
					DSTRING **name_dir);

/*
** returns a t_darr of matches with str
*/
t_darr			sh_cmp_darr(const t_darr darr, DSTRING *str);

/*
** returns the path written in the buffer by index or "."
*/
DSTRING			*sh_get_path(DSTRING *buf, size_t start_dir);

/*
** concatenates strings into one.
** if the special line “00” is connected between the lines,
** no separator will be inserted, if the special line “1 /” between the lines /
*/
char			*ft_concat(const size_t n, const char *spec, ...);

/*
** handles a special asterisk character
*/
t_regpath		help_get_regpath(const int fl, DSTRING *path);
DSTRING			*slice_reg(DSTRING *reg);
int				is_reg(DSTRING *buf);

/*
** auto-replace reg_expr
*/
void			loop(DSTRING *reg, int i, t_astr *rez, const int itr);

/*
** help reg_expr
*/
char			chek_astr(const DSTRING *reg);
DSTRING			*add_slash(char *str, DSTRING *reg);
char			is_strdot(const char *path);
DSTRING			*join_reg(DSTRING *n_dir, DSTRING *cmp, const char fl);
char			cmp_dirreg(DSTRING *n_dir, DSTRING *cmp,\
						DSTRING *reg, const char fl);
int				nmatch(const char *s1, const char *s2);

DSTRING			*get_dstr_rez(t_astr rez);

/*
** parsing a regular expression if there is a path in it,
** returns the path, if there is no path, returns "."
*/
t_regpath		get_regpath(DSTRING *reg);

/*
** To enable history management, click the up or down arrow
** ctrl+r search history
** tab	  show overlap history
*/

int				get_history_fd(int flags, char *er_context, ENV *envr);

/*
** write history in file
*/
void			write_history(DSTRING *line);

/*
** history management
*/
t_indch			show_history(DSTRING **buf, t_indch indc);

t_indch			sh_search_his(DSTRING **buf, t_indch indch);

char			get_new_histr(t_darr *histr, ENV *envr);

/*
** overwrites the command history file to avoid buffer overflow
*/
void			rewrite_histr(t_darr *histr, ENV *envr);

/*
** reads history file and fill struct t_darr
*/
char			get_histr(t_darr *histr, ENV *envr);

void			sys_term_init(void);
void			sys_term_restore(void);
void			sys_term_noecho(void);

void			add_buf_history(DSTRING *buf);
void			save_histr(ENV *envr);
void			del_history_buf(t_darr *histr);

#endif
