/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_test.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 13:24:58 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/26 17:51:26 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLTN_TEST_H
# define BLTN_TEST_H

# include "bltn.h"

typedef struct		s_flags_file
{
	char			f_blckdev;//
	char			f_symbdev;//
	char			f_dir;//
	char			f_exst;//
	char			f_file;//
	char			f_segid;//
	char			f_symb;//
	char			f_stbit;//
	char			f_chnl;//
	char			f_read;//
	char			f_notnull;//
	char			f_sckt;//
	char			f_suid;//
	char			f_write;//
	char			f_exec;//
	char			fd;
	char			s_notnull;
	char			s_null;	
}					t_flags_file;


/*
** 1 - =
** 2 - != 
** 3 - eq
** 4 - ge
** 5 - gt
** 6 - le
** 7 - lt
** 8 - ne
** 9 - ef
** 10 - nt
** 11 - ot
*/

typedef	struct		s_eq_args
{
	DSTRING			*first_arg;
	DSTRING			*secon_arg;
	int				acti;
}					t_eq_args;


int					bltn_test_prsfileflgs(t_flags_file *flgs, char *str);
int					bltn_test_file(char **args, t_flags_file *fl);
int					bltn_test_expr(char **args);
int					bltn_test_checkfile(char **args, t_flags_file *fl);
int					bltn_test_makeexpr(char **argv);
int					test_exp_doact(t_eq_args *exp);

#endif