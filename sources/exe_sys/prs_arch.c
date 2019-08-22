/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 04:13:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/22 23:49:50 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

int			prs_is_a_instruction(t_tok *tok)
{
	if (tok && tok->type != pipe_tk && \
	!(tok->type >= sep_tk && tok->type <= and_tk) && \
	tok->type != eof_tk)
		return (1);
	return (0);
}

char		**prs_args(t_dlist *tokens)
{
	char	**args;
	size_t	argslen;
	size_t	i;
	t_dlist	*it;
	t_tok	*tok;

	argslen = 0;
	it = tokens;
	while (it && prs_is_a_instruction((tok = it->content)) && ++argslen)
		it = it->next;
	if (!(args = ft_memalloc(sizeof(char*) * (argslen + 1))))
		return (0); //!MALLOC CALL FAILED. (SU.)
	it = tokens;
	i = -1;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type == expr_tk)
			if (!(args[++i] = ft_strdup(tok->value)))
				return (0); //! MALLOC CALL FAILED. (NOT SAFE, MAY BE LEAKS).
		it = it->next;
	}
	return (args);
}

t_rdtype		prs_rdr_type(t_tok *tok)
{
	if (tok->type == rd_a_tk)
		return (a_rdr);
	if (tok->type == rd_r_tk)
		return (r_rdr);
	if (tok->type == rd_w_tk)
		return (w_rdr);
	return (rw_rdr);
}

REDIRECT	*prs_rdr_wa(t_dlist *tokens)
{
	REDIRECT	*redir;
	t_tok		*tks[3];

	tks[0] = tokens->prev ? tokens->prev->content : 0;
	tks[1] = tokens->content;
	tks[2] = tokens->next ? tokens->next->content : 0;
	if (!(redir = ft_memalloc(sizeof(REDIRECT))))
		return (0);
	redir->type = prs_rdr_type(tks[1]);
	if (!(tks[0]) || tks[0]->type != fd_tk)
		redir->fdl = 1;
	else if (tks[0] && tks[0]->type == fd_tk)
		redir->fdl = ft_atoi(tks[0]->value);
	if (tks[2] && tks[2]->type == filename_tk && (redir->fdr = -1))
	{
		if (!(redir->file = ft_strdup(tks[2]->value)))
			free(redir);
	}
	else if (tks[2] && tks[2]->type)
		redir->fdr = ft_atoi(tks[2]->value);
	return (redir);
}

REDIRECT	*prs_rdr_r(t_dlist *tokens)
{
	REDIRECT	*redir;
	t_tok		*tks[3];

	tks[0] = tokens->prev ? tokens->prev->content : 0;
	tks[1] = tokens->content;
	tks[2] = tokens->next ? tokens->next->content : 0;
	if (!(redir = ft_memalloc(sizeof(REDIRECT))))
		return (0);
	redir->type = prs_rdr_type(tks[1]);
	if (!(tks[0]) || tks[0]->type != fd_tk)
		redir->fdl = 0;
	else if (tks[0] && tks[0]->type == fd_tk)
		redir->fdl = ft_atoi(tks[0]->value);
	if (tks[2] && tks[2]->type == filename_tk && (redir->fdr = -1))
	{
		if (!(redir->file = ft_strdup(tks[2]->value)))
			free(redir);
	}
	else if (tks[2] && tks[2]->type)
		redir->fdr = ft_atoi(tks[2]->value);
	return (redir);
}

REDIRECT	*prs_rdr_rw(t_dlist *tokens)
{
	REDIRECT	*redir;
	t_tok		*tks[3];

	tks[0] = tokens->prev ? tokens->prev->content : 0;
	tks[1] = tokens->content;
	tks[2] = tokens->next ? tokens->next->content : 0;
	if (!(redir = ft_memalloc(sizeof(REDIRECT))))
		return (0);
	redir->type = prs_rdr_type(tks[1]);
	if (!(tks[0]) || !(tks[1]))
	{
		free(redir);
	}
	else if (tks[0] && tks[0]->type == fd_tk)
		redir->fdl = ft_atoi(tks[0]->value);
	if (tks[2] && tks[2]->type == filename_tk && (redir->fdr = -1))
	{
		if (!(redir->file = ft_strdup(tks[2]->value)))
			free(redir);
	}
	else if (tks[2] && tks[2]->type)
		redir->fdr = ft_atoi(tks[2]->value);
	return (redir);
}

t_dlist		*prs_new_rdr_cr(t_dlist	*tokens)
{
	t_tok		*tok;
	REDIRECT	*redir;
	t_dlist		*res;

	tok = tokens->content;
	redir = 0;
	redir = (tok->type == rd_w_tk || tok->type == rd_a_tk) \
		? prs_rdr_wa(tokens) : redir;
	redir = (tok->type == rd_r_tk) ? prs_rdr_r(tokens) : redir;
	redir = (tok->type == rd_rw_tk) ? prs_rdr_rw(tokens) : redir;
	if (!redir || !(res = ft_dlstnew_cc(0, sizeof(REDIRECT))))
		return (0); //! MALLOC OR PARSE ERROR!
	res->content = redir;
	return (res);
}

t_dlist		*prs_rdrs(t_dlist **tokens)
{
	t_dlist		*rdrs;
	t_tok		*tok;
	t_dlist		*it;
	t_dlist		*new_rdr;

	it = *tokens;
	rdrs = 0;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type >= rd_w_tk && tok->type <= rd_rw_tk)
		{
			if (!(new_rdr = prs_new_rdr_cr(it)))
				it = 0; //! MALLOC FAILED CALL
			ft_dlstpush(&rdrs, new_rdr);
		}
		it = it->next;
	}
	*tokens = it;
	return (rdrs);
}

t_dlist		*prs_expr(ETAB **tab, t_dlist *tokens)
{
	EXPRESSION		*expr; //* Указатель на выражение
	ETAB			*nrow; //* Указатель на новый ряд таблицы исполнения.

	if (!(nrow = (ETAB *)ft_dlstnew_cc(0, 0)))
		return (0); //! MALLOC CALL FAILED. (SU.)
	ft_dlstpush((t_dlist **)tab, (t_dlist *)nrow);
	nrow->type = ET_EXPR;
	if (!(nrow->instruction = ft_memalloc(sizeof(EXPRESSION))))
		return (0); //! MALLOC CALL FAILED. (SU.)
	expr = (EXPRESSION *)nrow->instruction;
	//* Если предыдущая запись это пайп, то заносим его адресс в дескриптор выражения
	if (nrow->prev_e && nrow->prev_e->type == ET_PIPE)
		expr->ipipe_fds = ((PIPE *)nrow->prev_e->instruction)->pirw;
	//* Получаем аргументы командной строки
	//TODO: Написать парсинг аргументов
	if (!(expr->args = prs_args(tokens)))
		return (0); //! MALLOC CALL FAILED. (NOT SAFE.)
	//* Получаем список перенаправлений
	//TODO: Написать парсинг перенаправлений и передвинуть токенс на след токен
	expr->redirections = prs_rdrs(&tokens);
	return (tokens);
}

static t_dlist	*prs_pipe(ETAB **tab, t_dlist *tk)
{
	ETAB		*curt;
	EXPRESSION	*expr;
	PIPE		*pipee;

	// ВЫДЕЛЕНИЕ ПАМЯТИ НА ТАБЛИЦУ
	curt = (ETAB *)ft_dlstnew_cc(0, 0);
	ft_dlstpush((t_dlist **)tab, (t_dlist *)curt);
	curt->type = ET_PIPE;
	if (!(curt->instruction = ft_memalloc(sizeof(PIPE))))
		return (0); //! MALLOC FAILED
	expr = curt->prev_e->instruction;
	pipee = curt->instruction;
	expr->opipe_fds = pipee->pirw;
	return (tk->next);
}

void		sh_tparse(t_dlist *tokens, char **envp)
{
	ETAB		*etab;
	ETAB		*trash;
	t_tok		*token;

	etab = 0;
	while (tokens && (token = tokens->content))
	{
		if (token->type == sep_tk || token->type == eof_tk)
		{
			trash = 0;
			while (etab != 0)
			{
				ft_dlstunshift((t_dlist **)&trash, ft_dlstshift((t_dlist **)&etab));
				if (trash->type == ET_EXPR)
					exe_execute_pi(trash->instruction, envp, 0);
			}
			exe_wait_cps();
			ft_dlst_delf((t_dlist **)&trash, (size_t)-1, et_rm_ett);
		}
		tokens = token->type == eof_tk || token->type == sep_tk ? tokens->next : tokens;
		tokens = token->type == expr_tk ? prs_expr(&etab, tokens) : tokens;
		tokens = token->type == pipe_tk ? prs_pipe(&etab, tokens) : tokens;
	}
	if (etab)
		ft_dlst_delf((t_dlist **)&etab, (size_t)-1, et_rm_ett);
	return ; //! MALLOC CALL FAILED.
}
