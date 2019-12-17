/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 08:41:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/17 16:22:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "exe_prs_defines.h"
# include "rms.h"
# include "ft_dlist.h"

char		*ft_basename(const char *path);
size_t		sizeof_warr(char **arr);
t_dlist		*sh_tparse(t_dlist *tokens, ENV *vars, t_tk_type end_tk, \
int *status);
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

t_rdtype	prs_rdr_type(t_tok *tok);

/*
** Function creates a new t_dlist with REDIRECTION DESC.
*/

t_dlist		*prs_new_rdr_cr(t_dlist *tokens, ENV *envs);
int			prs_hrd_word(t_dlist *tl, REDIRECT *redr);
int			prs_rdr_fdr_file(t_dlist *tokens, REDIRECT *redir, ENV *envr);
int			prs_rdr_fdl(t_dlist *tokens, REDIRECT *redir);
int			prs_rdr_is_std_fd(t_dlist *fd_tl);
char		*rdr_get_filename(t_dlist *tks, ENV *envr);

/*
** Function returns a full double-list with RDR DESCS
*/

t_dlist		*prs_rdrs(t_dlist **tokens, ENV *envs);

/*
** CREATE REDIRECT_DS FOR '<>'
*/

REDIRECT	*prs_rdr_rw(t_dlist *tokens, ENV *envs);

/*
** CREATE REDIRECT_DS FOR '<'
*/

REDIRECT	*prs_rdr_r(t_dlist *tokens, ENV *envs);

/*
** CREATE REDIRECT_DS FOR '> >>'
*/

REDIRECT	*prs_rdr_wa(t_dlist *tokens, ENV *envs);

/*
** PARSING OF A EXPRESSIONS ETT
*/

t_dlist		*prs_expr(ETAB **tab, t_dlist *tokens, ENV *envs);
t_dlist		*prs_lambda(t_dlist *tks, ENV *envr, ETAB **etab);
/*
** PARSING OF A PIPES ETT
*/

t_dlist		*prs_pipe(ETAB **tab, t_dlist *tk);
t_dlist		*prs_math(ETAB **tab, t_dlist *tokens, ENV *envs);
t_dlist		*prs_subsh(ETAB **tab, t_dlist *tokens, ENV *envs);
t_dlist		*prs_bkgr(ETAB **tab, t_dlist *tokens, ENV *envr);
t_dlist		*arg_sub(t_dlist *tokens, char **args, size_t ind, ENV *envr);

/*
** TOKEN SKIPERS (returns an pointer ro a t_dlist (end_token))
*/

t_dlist		*prs_skip_paired_trg(t_dlist *tks, t_tk_type op, \
t_tk_type cls, int cntr);
t_dlist		*prs_skip_until(t_dlist *tks, t_tk_type brk_toks);
t_dlist		*prs_skip_paired(t_dlist *tks, t_tk_type op, t_tk_type cls);
t_dlist		*arg_tok_r_skip(t_dlist *tokens, t_tk_type flags);
t_dlist		*arg_tok_skip(t_dlist *tokens, t_tk_type flags);

/*
** PARSING AND AND OR TOKENS
*/

t_dlist		*prs_and(t_dlist *tks, ENV *envr, int *status);
t_dlist		*prs_or(t_dlist *tks, ENV *envr, int *status);

/*
** PARSING FUNCTIONS
*/

t_dlist		*prs_func(t_dlist *tks, ENV *envr);

/*
** PARSING ASSIGNMENTS
*/

t_dlist		*prs_assigm(t_dlist *tks, ENV *envs, int *status);

/*
** PARSING FLOWS
*/

t_dlist		*prs_while(t_dlist *tks, ENV *envs, int *status);
t_dlist		*prs_if(t_dlist *tks, ENV *envs, int *status);
t_dlist		*prs_for(t_dlist *tks, ENV *envs, int *status);
char		*prs_forin_vals(t_dlist *tks, ENV *env);

/*
** GET DEREFERENCING
*/

char		*get_deref_name(char *key, ENV *envr);
char		*get_deref_math(char *expr, ENV *envr);
char		*get_deref(t_dlist *tokens, ENV *envr);
char		*get_deref_subsh(char *code, ENV *envr);
char		*prc_substitute(char *code, ENV *envr, int is_in);
int			prs_error_handler(int ecode, size_t type, \
ENV *envr, EXPRESSION *expr);
int			prs_set_last_status(int *status, ENV *envr);

t_dlist		*args_get_next_arg(t_dlist *tks);

#endif
