/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_prs_defines.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 07:22:43 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 07:33:39 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_PRS_DEFINES_H
# define EXE_PRS_DEFINES_H

# include "ft_dlist.h"
# include "env.h"
# include "sys_tools/sys_errors.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# ifdef __linux__
#  include <sys/wait.h>
# endif
# include "ft_mem.h"
# include "sh_token.h"
# include "ft_string.h"
# include "ft_avl_tree.h"

# define ETAB t_extab

# define ET_EXPR (size_t)1
# define ET_PIPE (size_t)2
# define ET_MATH (size_t)3
# define ET_SUBSH (size_t)4
# define ET_BCKGR (size_t)5

# define EXPRESSION t_command
# define REDIRECT t_redir
# define PIPE t_pipe
# define MATH t_math
# define FUNC t_func
# define SUBSH t_subsh
# define BCKGR t_bkgr

# define INTERRUPT_CALL (t_dlist *)0xFFFFFFFFFFFFFFFF

typedef enum		e_rd_type
{
	r_rdr = 1,
	w_rdr,
	rw_rdr,
	a_rdr,
	herd
}					t_rdtype;

typedef struct		s_redirection
{
	char			*file;
	int				fdl;
	int				fdr;
	t_rdtype		type;
}					t_redir;

typedef struct		s_command
{
	char			**args;
	int				*ipipe_fds;
	int				*opipe_fds;
	t_dlist			*redirections;
}					t_command;

typedef struct		s_math
{
	char			*expr;
	int				*ipipe_fds;
	int				*opipe_fds;
	t_dlist			*redirections;
}					t_math;

typedef struct		s_subsh
{
	void			*commands;
	int				*ipipe_fds;
	int				*opipe_fds;
	t_dlist			*redirections;
}					t_subsh;

typedef struct		s_bkgr
{
	t_dlist			*tls;
}					t_bkgr;

typedef struct		s_pipe
{
	int				pirw[2];
}					t_pipe;

typedef struct		s_42func
{
	t_dlist			*func_code;
}					t_func;

typedef struct		s_extab
{
	void			*instruction;
	size_t			type;
	struct s_extab	*next_e;
	struct s_extab	*prev_e;
}					t_extab;

#endif
