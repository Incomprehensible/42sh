/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 08:41:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 11:00:37 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ft_dlist.h"
# include "env.h"

# define EXPRESSION t_command
# define REDIRECT t_redir
# define PIPE t_pipe
# define MATH t_math
# define FUNC t_func
# define SUBSH t_subsh

typedef enum		e_rd_type		// TYPES OF REDIRS
{
	r_rdr, // 0<
	w_rdr, // 1>
	rw_rdr, // <>
	a_rdr // 1>>
}					t_rdtype;

typedef struct		s_redirection	// REDIR DESCRIPTOR!!
{
	char			*file;			// IF file-type redirection, name of file			// IF proc redirection, string of shell
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

typedef struct		s_math			// ARIPHMETICAL DESCRIPTOR!!
{
	char			*expr;
	int				*ipipe_fds;		// input pipe
	int				*opipe_fds;		// output pipe
	t_dlist			*redirections;	// double-list with a content of t_redir (REDIRECT)				
}					t_math;

typedef struct		s_subsh
{
	void			*commands;
	int				*ipipe_fds;
	int				*opipe_fds;
	t_dlist			*redirections;
}					t_subsh;

typedef struct		s_pipe			// PIPE DESCRIPTOR
{
	int				pirw[2];
}					t_pipe;

typedef struct		s_42func
{
	t_dlist			*func_code;
}					t_func;

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
# include "executer.h"
# include "rms.h"

char		*ft_basename(const char *path);

t_dlist		*sh_tparse(t_dlist *tokens, ENV *vars, t_tk_type end_tk, int *status);
char		*sh_checkbins(const char *cmd, ENV *vars, pid_t *pid);

/*
** FUNCTION RETURNS TRUE IF THERE IS NO EXPR SEPS
*/
int			prs_is_a_instruction(t_tok *tok);

/*
** FUNCTION RETURNS ARGS WARR FOR EXPRESSIONS
*/
char		**prs_args(t_dlist *tokens, ENV *envs);

/*
** FUNCTION RETURS TYPE FOR REDIRECTION DESCRIPTOR
*/
t_rdtype		prs_rdr_type(t_tok *tok);

/*
** Function creates a new t_dlist with REDIRECTION DESC.
*/
t_dlist			*prs_new_rdr_cr(t_dlist *tokens, ENV *envs);

/*
** Function returns a full double-list with RDR DESCS
*/
t_dlist			*prs_rdrs(t_dlist **tokens, ENV *envs);

/*
** CREATE REDIRECT_DS FOR '<>'
*/
REDIRECT		*prs_rdr_rw(t_dlist *tokens, ENV *envs);

/*
** CREATE REDIRECT_DS FOR '<'
*/
REDIRECT		*prs_rdr_r(t_dlist *tokens, ENV *envs);

/*
** CREATE REDIRECT_DS FOR '> >>'
*/
REDIRECT		*prs_rdr_wa(t_dlist *tokens, ENV *envs);

/*
** PARSING OF A EXPRESSIONS ETT
*/
t_dlist			*prs_expr(ETAB **tab, t_dlist *tokens, ENV *envs);

/*
** PARSING OF A PIPES ETT
*/
t_dlist			*prs_pipe(ETAB **tab, t_dlist *tk);

t_dlist			*prs_math(ETAB **tab, t_dlist *tokens, ENV *envs);
t_dlist			*prs_subsh(ETAB **tab, t_dlist *tokens, ENV *envs);
t_dlist			*arg_sub(t_dlist *tokens, char **args, size_t ind, ENV *envr);

/*
** TOKEN SKIPERS (returns an pointer ro a t_dlist (end_token))
*/
t_dlist			*prs_skip_paired_trg(t_dlist *tks, t_tk_type op, \
t_tk_type cls, int cntr);
t_dlist			*prs_skip_until(t_dlist *tks, t_tk_type brk_toks);
t_dlist			*prs_skip_paired(t_dlist *tks, t_tk_type op, t_tk_type cls);
t_dlist			*arg_tok_r_skip(t_dlist *tokens, t_tk_type flags);
t_dlist			*arg_tok_skip(t_dlist *tokens, t_tk_type flags);

/*
** PARSING AND AND OR TOKENS
*/
t_dlist			*prs_and(t_dlist *tks, ENV *envr, int *status);
t_dlist			*prs_or(t_dlist *tks, ENV *envr, int *status);

/*
** PARSING FUNCTIONS
*/
t_dlist			*prs_func(t_dlist *tks, ENV *envr);

/*
** PARSING ASSIGNMENTS
*/
t_dlist			*prs_assigm(t_dlist *tks, ENV *envs, int *status);

/*
** PARSING FLOWS
*/
t_dlist			*prs_while(t_dlist *tks, ENV *envs, int *status);
t_dlist			*prs_if(t_dlist *tks, ENV *envs, int *status);
t_dlist			*prs_for(t_dlist *tks, ENV *envs, int *status);

char			*get_deref_subsh(char *code, ENV *envr);
char			*prc_substitute(char *code, ENV *envr, int is_in);

int				prs_error_handler(int ecode, int *stat, ENV *envr, EXPRESSION *expr);

int				env_core_set(char *key, char *value, t_avl_tree *core);
int				prs_set_last_status(int *status, ENV *envr);

#endif
