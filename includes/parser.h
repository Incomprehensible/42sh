/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 08:41:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/17 15:40:35 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "ft_dlist.h"
# include "ft_mem.h"
# include "ft_string.h"
# include "ft_avl_tree.h"

# define EXPRESSION t_command
# define REDIRECT t_redir
# define IF t_if
# define WHILE t_while
# define FUNCTION t_function
# define PIPE t_pipe
# define EXTAB t_extab

# define ETT_EXPR (size_t)1
# define ETT_IF (size_t)2
# define ETT_WHL (size_t)3
# define ETT_FUNC (size_t)4
# define ETT_PIPE (size_t)5

typedef struct		s_extab			// RELOADING OF T_DLIST
{
	void			*ex;
	size_t			type;
	struct s_extab	*next_e;
	struct s_extab	*prev_e;
}					t_extab;

typedef enum		e_rd_type		// TYPES OF REDIRS
{
	r_rdr, // 0<
	w_rdr, // 1>
	rw_rdr, // <>
	a_rdr // 1>>
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
	t_extab			*cond_e;		// COND EXPR
	t_extab			*true_e;		// EXPR IF TRUE
	t_extab			*fls_e;			// EXPR IF FALSE
	t_extab			*next_e;		// AFTER COMMANDS
}					t_if;

typedef struct		s_while			// WHILE DESCRIPTOR (RESERVED)
{
	t_extab			*cond_e;		// COND EXPR
	t_extab			*cnt_e;			// EXPR FOR LOOP
	t_extab			*next_e;		// AFTER COMMANDS
}					t_while;

typedef struct		s_function		// WHILE DESCRIPTOT (RESERVED)
{
	t_extab			*cmd_e;			// CMDS_START
	t_avl_tree		*f_locals;		// LOCAL_VARIABLES
}					t_function;

typedef struct		s_pipe			// PIPE DESCRIPTOR
{
	int				pirw[2];
}					t_pipe;

EXTAB				*parser_get_from_tklist(t_dlist *tokens);


#endif
