/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_req.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 09:09:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/14 10:11:49 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_REQ_H
# define EXE_REQ_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "ft_dlist.h"
# include "ft_avl_tree.h"

# define EXPRESSION t_command
# define REDIRECT t_redir
# define IF t_if
# define WHILE t_while
# define FUNCTION t_function

typedef enum		e_rd_type		// TYPES OF REDIRS
{
	r_rdr,
	w_rdr,
	rw_rdr,
	a_rdr
}					t_rdtype;

typedef struct		s_redirection	// REDIR DESCRIPTOR!!
{
	char			*file;			// IF file-type redirection, name of file
	int				fdl;			// left fd
	int				fdr;			// right fd
	t_rdtype		type;			// type of redirection
}					t_redir;

typedef struct		s_command		// COMMAND DESCRIPTOR!!
{
	char			**args;			// Command line arguments
	int				*ipipe_fds;		// input pipe
	int				*opipe_fds;		// output pipe
	t_dlist			*redirections;	// double-list with a content of t_redir (REDIRECT)
}					t_command;

typedef struct		s_if			// IF DESCRIPTOR (RESERVED)
{
	t_dlist			*cond_e;		// COND EXPR
	t_dlist			*true_e;		// EXPR IF TRUE
	t_dlist			*fls_e;			// EXPR IF FALSE
	t_dlist			*next_e;		// AFTER COMMANDS
}					t_if;

typedef struct		s_while			// WHILE DESCRIPTOR (RESERVED)
{
	t_dlist			*cond_e;		// COND EXPR
	t_dlist			*cnt_e;			// EXPR FOR LOOP
	t_dlist			*next_e;		// AFTER COMMANDS
}					t_while;

typedef struct		s_function		// WHILE DESCRIPTOT (RESERVED)
{
	t_dlist			*cmd_e;			// CMDS_START
	t_avl_tree		*f_locals;		// LOCAL_VARIABLES
}					t_function;

/*
** BLTN FUNC
** Functions executes command with COMMAND_DESCRIPTOR.
** It creates pipe if OUTPUT_PIPE pointer is not 0, and close INPUT_PIPES
** in parrent proc after all;
** Creates redirections following the redir_descriptor;
** Returns a child's pid.
*/
int			exe_execute_pi(EXPRESSION *cmd, char **envp);
void		exe_redir_ex(REDIRECT *rdr);
int			exe_wait_cps(void);

#endif
