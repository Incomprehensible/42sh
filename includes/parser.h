/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 08:41:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/27 15:20:52 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ft_dlist.h"
# include "env.h"

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
# include "sh_token.h"
# include "ft_string.h"
# include "ft_avl_tree.h"
# include "executer.h"
# include "rms.h"

t_dlist		*sh_tparse(t_dlist *tokens, ENV *vars, t_tk_type end_tk, int *status);
char		*sh_checkbins(const char *cmd, ENV *vars);

/*
** FUNCTION RETURNS TRUE IF THERE IS NO EXPR SEPS
*/
int			prs_is_a_instruction(t_tok *tok);

/*
** FUNCTION RETURNS ARGS WARR FOR EXPRESSIONS
*/
char		**prs_args(t_dlist **tokens, ENV *envs);

/*
** FUNCTION RETURS TYPE FOR REDIRECTION DESCRIPTOR
*/
t_rdtype		prs_rdr_type(t_tok *tok);

/*
** Function creates a new t_dlist with REDIRECTION DESC.
*/
t_dlist			*prs_new_rdr_cr(t_dlist *tokens);

/*
** Function returns a full double-list with RDR DESCS
*/
t_dlist			*prs_rdrs(t_dlist **tokens);

/*
** CREATE REDIRECT_DS FOR '<>'
*/
REDIRECT		*prs_rdr_rw(t_dlist *tokens);

/*
** CREATE REDIRECT_DS FOR '<'
*/
REDIRECT		*prs_rdr_r(t_dlist *tokens);

/*
** CREATE REDIRECT_DS FOR '> >>'
*/
REDIRECT		*prs_rdr_wa(t_dlist *tokens);

/*
** PARSING OF A EXPRESSIONS ETT
*/
t_dlist			*prs_expr(ETAB **tab, t_dlist *tokens, ENV *envs);

/*
** PARSING OF A PIPES ETT
*/
t_dlist			*prs_pipe(ETAB **tab, t_dlist *tk);

#endif
