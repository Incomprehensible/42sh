/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 09:09:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/29 22:10:39 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include "exe_prs_defines.h"
# include "sh_tokenizer.h"
# include "rms.h"

/*
** BLTN FUNC
** Functions executes command with COMMAND_DESCRIPTOR.
** It creates pipe if OUTPUT_PIPE pointer is not 0, and close INPUT_PIPES
** in parrent proc after all;
** Creates redirections following the redir_descriptor;
** Returns a child's pid.
*/
int			subsh_tok_err(char *contx, int rcode, t_dlist **tks, ENV *envr);
int			exe_execute_pi(EXPRESSION *cmd, ENV *vars);
int			exe_redir_ex(REDIRECT *rdr, ENV *envr);
int			exe_execute_expr(EXPRESSION *expr, ENV *envr, int *status);
int			exe_subshell_expr(SUBSH *subsh, ENV *envr, int *status);
int			exe_one_command_lnch(SUBSH *subsh, t_dlist *tl, ENV *envr, int *st);
int			exe_bkgr_expr(BCKGR *bkgr, ENV *envr, int *status);
int			sbsh_is_fork_n_need(t_dlist *tok, t_dlist *rdrs);

int			exe_redir_list(t_dlist *lst, ENV *envr);
int			exe_heredoc_check(t_dlist *lst, ENV *envr);

void		exe_redir_save420(t_dlist *redrs);
void		exe_redir_load420(t_dlist *redrs);
int			exe_execute_b(EXPRESSION *expr, ENV *envr, \
int (*bltn)(char **, ENV *), int *status);
int			exe_execute_pi_b(EXPRESSION *expr, ENV *envr, \
int (*bltn)(char **, ENV *));
long		do_math_bltn(char *str_xp, ENV *envr, ERR *err);
int			exe_execute_f(EXPRESSION *expr, ENV *envr, FUNC *func, int *status);
int			exe_execute_pi_f(EXPRESSION *expr, ENV *envr, FUNC *func);
int			exe_rdr_heredocument(REDIRECT *rdr);
int			pex_g_ptab_pgid(pid_t cpid, char *i_line);
int			prs_executor(ETAB **etab, ENV *envs);
int			prs_etab_handlers(ETAB **etab_row, \
ETAB **pipe_cache, int *status, ENV *envr);
int			prs_post_execute_xp(int *status, pid_t cpid, ENV *env);

#endif
