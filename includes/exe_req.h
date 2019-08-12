/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_req.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 09:09:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/12 13:04:43 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_REQ_H
# define EXE_REQ_H

# include <unistd.h>
# include <stdlib.h>
# include "ft_dlist.h"

# define COMMAND t_command
# define REDIRECT t_redir

typedef enum		e_rd_type		// TYPES OF REDIRS
{
	rd_read,						// <&
	rd_read_file,					// <
	rd_write,						// >
	rd_write_file,					// >&
	rd_append,						// >>
	rd_rw_open						// <>
}					t_rdtype;

typedef struct		s_redirection	// REDIR DESCRIPTOR!!
{
	char			*filepath;		// IF file-type redirection, name of file
	int				fds[2];			// [0][>/<][1]. (-1)==stdin + stderr
	t_rdtype		type;			// type of redirection
}					t_redir;

typedef struct		s_command		// COMMAND DESCRIPTOR!!
{
	char			**args;			// Command line arguments
	int				*ipipe_fds;		// input pipe
	int				*opipe_fds;		// output pipe
	t_dlist			*redirections;	// double-list with a content of t_redir (REDIRECT)
}					t_command;

/*
** BLTN FUNC
** Functions executes command with COMMAND_DESCRIPTOR.
** It creates pipe if OUTPUT_PIPE pointer is not 0, and close INPUT_PIPES
** in parrent proc after all;
** Creates redirections following the redir_descriptor;
** Returns a child's pid.
*/
int			exe_execute_pi(COMMAND *cmd, char **envp);

#endif
