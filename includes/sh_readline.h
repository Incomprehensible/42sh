/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:53:02 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/17 15:00:22 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_READLINE_H
# define SH_READLINE_H

# include <termcap.h>
# include <stdio.h>
# include "dstring.h"
# include "ft_avl_tree.h"
# include "libft.h"


# define S_DARR_STRINGS		2096

typedef	struct	s_envp
{
	t_avl_tree	*global;
	t_avl_tree	*local;
}				t_envp;

/*	strings - array of DSTRING'S(every string is name of command / directory)
	maxlen  - max length name command / directory
	count   - count elements in array
	allsize - count symbols all elements in array */
typedef struct	s_darr
{
	DSTRING		*strings[S_DARR_STRINGS];
	size_t 		maxlen;
	size_t 		count;
	size_t 		allsize;
}				t_darr;

/* INIT GLOBAL AND LOCAL ENV
	argc - count of arguments from main()
	argv - arguments from main()
	envp - envp from main()
	env  - adreess for env*/
void			init_vars(int argc, char **argv, char **envp, t_envp *env);

/*	GET ALL CMDS FROM $PATH */
t_darr			*get_list_cmds(t_envp	*envp);

char			*sh_readline(void);
void			sh_rewrite(const DSTRING *buf, const size_t index);


ssize_t			sh_del_char(DSTRING **buf, size_t index, const char flag);
ssize_t			dstr_del_char(DSTRING **src, ssize_t n);
ssize_t			sh_dstr_iscmd(const DSTRING *str);
ssize_t			dstrrchr(const DSTRING *src, const int ch);
DSTRING			*dstr_insert_cut(DSTRING *dst, DSTRING *src, ssize_t ind);
DSTRING			*dstr_insert_scut(DSTRING *dst, char *src, ssize_t ind);
DSTRING			*dstr_insert_ccut(DSTRING *dst, char src, ssize_t ind);

t_darr			sh_dir_content(char *path);
char			sh_isdir(DSTRING *buf, ssize_t start_dir);
char			sh_check_dot(const DSTRING *path);

void			free_darr(DSTRING **darr);
void			free_darr_n(DSTRING **darr, const size_t size);

size_t			sh_esc(size_t index, const size_t max);
int				ft_getch(void);


char			*ft_concat(const size_t n, const char *spec, ...);

#endif
