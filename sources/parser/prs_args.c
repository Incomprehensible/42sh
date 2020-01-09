/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:17:54 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/17 15:19:58 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "ft_lbuffer.h"
#include "sys_tools/sys_tools.h"
#include "parser.h"

size_t			args_count(t_dlist *tokens)
{
	t_tok		*tok;

	tok = tokens->content;
	if (tok->type & (TK_EXPR | TK_DEREF | TK_MATH | TK_PROC_OUT | TK_PROC_IN))
	{
		while (0 == (((t_tok *)tokens->content)->type \
		& (TK_SEPS | TK_EMPTY | TK_FLOWS)))
			tokens = tokens->next;
		return (1 + args_count(tokens));
	}
	else if (((t_tok *)tokens->content)->type & (TK_SEPS | TK_FLOWS))
		return (0);
	else
		return (args_count(args_get_next_arg(tokens)));
}

t_dlist			*arg_sub_df(t_dlist *tokens, DSTRING *expr, ENV *env)
{
	char		*trimmed;
	char		*tmp;

	tokens = tokens->next;
	if (!(tmp = get_deref(tokens, env)))
		return (NULL);
	if (!(trimmed = ft_strtrim(tmp)))
		return (NULL);
	dstr_insert_str(expr, trimmed, MAX_LL);
	free(tmp);
	free(trimmed);
	return (tokens);
}

t_dlist			*arg_sub_pr_sb(t_dlist *tokens, \
DSTRING *expr, ENV *env, t_tok *tok)
{
	char		*tmp;

	if (!(tmp = prc_substitute(tok->value, env, \
	tok->type == TK_PROC_IN ? 1 : 0)))
		return (NULL);
	dstr_insert_str(expr, tmp, MAX_LL);
	free(tmp);
	return (tokens);
}

t_dlist			*arg_sub(t_dlist *tokens, char **args, size_t ind, ENV *envr)
{
	DSTRING		*expr_buff;
	t_tok		*tok;

	expr_buff = dstr_new("");
	tokens = args_get_next_arg(tokens);
	while (!((tok = tokens->content)->type & \
	(TK_SEPS | TK_FLOWS | TK_FDS_RDS | TK_EMPTY | TK_WORD | TK_HERED)))
	{
		if (tok->type == TK_EXPR || tok->type == TK_VALUE)
			dstr_insert_str(expr_buff, tok->value, MAX_LL);
		else if (tok->type == TK_DEREF)
			tokens = arg_sub_df(tokens, expr_buff, envr);
		else if (tok->type & (TK_PROC_OUT | TK_PROC_IN))
			tokens = arg_sub_pr_sb(tokens, expr_buff, envr, tok);
		if (!tokens)
		{
			dstr_del(&expr_buff);
			return (tokens);
		}
		tokens = tokens->next;
	}
	args[ind] = ft_strdup(expr_buff->txt);
	dstr_del(&expr_buff);
	return (tokens);
}

char			**prs_args(t_dlist *tokens, ENV *envs)
{
	char		**args;
	size_t		ind;
	size_t		argscount;

	argscount = args_count(tokens);
	args = (char **)ft_memalloc(sizeof(char *) * (argscount + 1));
	ind = -1;
	while (++ind < argscount)
		if (!(tokens = arg_sub(tokens, args, ind, envs)))
		{
			et_rm_warr(args);
			return (0);
		}
	return (args);
}
