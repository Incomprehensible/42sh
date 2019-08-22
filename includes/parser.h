/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 08:41:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/22 23:16:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ft_dlist.h"

# define EXPRESSION t_command
# define REDIRECT t_redir
# define PIPE t_pipe

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
	char			*sbsh;			// IF proc redirection, string of shell
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

typedef struct		s_pipe			// PIPE DESCRIPTOR
{
	int				pirw[2];
}					t_pipe;

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "ft_mem.h"
# include "ft_string.h"
# include "ft_avl_tree.h"
# include "executer.h"
# include "rms.h"

void		sh_tparse(t_dlist *tokens, char **envp);

#endif
