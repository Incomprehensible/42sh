/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_rdrs_support.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 09:41:56 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/14 19:29:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

char			*rdr_get_filename(t_dlist *tks, ENV *envr)
{
	t_dyn_string	*buff;
	char			*str;
	t_tok			*tok;

	buff = dstr_new("");
	while (tks && (tok = tks->content) && \
	(tok->type & (TK_DEREF | TK_PROF_IN | TK_PROF_OUT | TK_FILENAME)))
	{
		if ((TK_PROF_IN | TK_PROF_OUT) & tok->type)
			str = prc_substitute(tok->value, envr, \
			TK_PROF_IN == tok->type ? 1 : 0);
		else if (TK_FILENAME == tok->type)
			str = ft_strdup(tok->value);
		else if (TK_DEREF == tok->type)
			str = get_deref(\
			(tks = tks->next), envr);
		if (str)
		{
			dstr_insert_str(buff, str, buff->strlen);
			free(str);
		}
		tks = tks->next;
	}
	return (dstr_flush(&buff));
}

int				prs_rdr_is_std_fd(t_dlist *fd_tl)
{
	t_tok		*tok;

	tok = (t_tok *)fd_tl->content;
	if (tok->type != TK_FD)
		return (1);
	fd_tl = arg_tok_r_skip(fd_tl->prev, TK_EMPTY);
	if (!fd_tl || !fd_tl->content)
		return (0);
	tok = (t_tok *)fd_tl->content;
	if (tok->type & TK_RDS1)
		return (1);
	return (0);
}

int				prs_rdr_fdl(t_dlist *tokens, REDIRECT *redir)
{
	t_tok		*tok;

	tokens = arg_tok_r_skip(tokens, TK_EMPTY | TK_RDS1);
	if (!tokens || !(tok = tokens->content) || prs_rdr_is_std_fd(tokens))
	{
		redir->fdl = redir->type == r_rdr ? 0 : redir->fdl;
		redir->fdl = redir->type & (a_rdr | w_rdr) ? 1 : redir->fdl;
		return (0);
	}
	else if (tok->type == TK_FD)
	{
		redir->fdl = ft_atoi(tok->value);
		return (0);
	}
	return (-1);
}

int				prs_rdr_fdr_file(t_dlist *tokens, REDIRECT *redir, ENV *envr)
{
	t_tok		*tok;

	if (!(tokens = arg_tok_skip(tokens, TK_EMPTY | TK_RDS)))
		return (-2);
	tok = tokens->content;
	if (tok->type & (TK_PROF_IN | TK_PROF_OUT | TK_FILENAME | TK_DEREF))
	{
		redir->file = rdr_get_filename(tokens, envr);
		return (0);
	}
	if (tok->type == TK_FD)
	{
		redir->file = 0;
		redir->fdr = ft_atoi(tok->value);
		return (0);
	}
	return (-1);
}

int				prs_hrd_word(t_dlist *tl, REDIRECT *redr)
{
	t_tok		*tok;

	if (!(tl = arg_tok_skip(tl->next, TK_EMPTY)))
		return (-2);
	tok = tl->content;
	if (tok->type != TK_WORD)
		return (-1);
	if (!(redr->file = ft_strdup(tok->value)))
		sys_fatal_memerr("FAILED_MAKE_HEREDOC");
	redr->fdr = 0;
	redr->fdl = 0;
	return (0);
}
