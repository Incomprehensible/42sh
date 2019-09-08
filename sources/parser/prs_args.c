/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:17:54 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/08 20:43:16 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

/*
int				prs_deref_name(t_dlist **tks, ENV *envs)
{
	t_tok		*dtok;
	t_dlist		*tmp;
	DSTRING		*dstr;
	int			ret;

	tmp = (*tks)->next;
	ret = 0;
	ft_dlst_delcut(tks, free_token);
	(*tks) = tmp;
	dtok = tmp->content;
	dtok->type = TK_VALUE;
	if (!(dstr = env_get_variable(dtok->value, envs)))
		return (-1);
	free(dtok->value);
	if (!(dtok->value = ft_strdup(dstr->txt)))
		ret = -1; // ERROR: prs_deref: Malloc failed.
	dstr_del(&dstr);
	return (ret);
}

int				prs_deref_math(t_dlist **tks, ENV *envs)
{
	t_tok		*dtok;
	char		**exprs;
	t_dlist		*tmp;
	long		res;
	int			ret;

	tmp = (*tks)->next;
	ret = 0;
	ft_dlst_delcut(tks, free_token);
	(*tks) = tmp;
	dtok = tmp->content;
	dtok->type = TK_VALUE;
	exprs = ft_strsplits(dtok->value, ",");
	// TODO: unsafe malloc.
	while (exprs[ret])
		res = do_math_bltn(exprs[ret++], envs);
	et_rm_warr(exprs);
	free(dtok->value); 
	if (!(dtok->value = ft_lltoa_base(res, 10)))
		ret = -1; // ERROR: prs_deref: Malloc failed.
	return (0);
}

int				prs_vars_derefs(t_dlist *tks, ENV *envs)
{
	t_tok		*tok;
	t_dlist		*it;

	it = tks;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type == TK_DEREF)
		{
			tok = it->next->content;
			if (tok->type == TK_NAME && (prs_deref_name(&it, envs) < 0))
				return (-1); // ERROR: prs_deref: Malloc failed.
			if (tok->type == TK_MATH && (prs_deref_math(&it, envs) < 0))
				return (-1); // ERROR: prs_deref: Malloc failed.
		}
		it = it->next;
	}
	return (0);
}

int				prs_join_exprs(t_dlist *tks)
{
	t_tok		*tok[2];
	t_dlist		*tmp;
	char		*newval;

	tok[0] = tks->content;
	tok[1] = tks->next->content;
	tmp = tks->next;
	if (!(newval = ft_strjoin(tok[0]->value, tok[1]->value)))
		return (-1);
	ft_dlst_delcut(&tmp, free_token);
	free(tok[0]->value);
	tok[0]->value = newval;
	tok[0]->type = TK_EXPR;
	return (1);
}

int				prs_values_joins(t_dlist *tks)
{
	t_tok		*tok;
	t_tok		*ntok;
	t_dlist		*ntks;

	while (tks && tks->next && prs_is_a_instruction((tok = tks->content)))
	{
		ntks = tks->next;
		ntok = ntks->content;
		if (tok->type == TK_VALUE && (ft_strlen(tok->value) == 0))
		{
			ft_dlst_delcut(&tks, free_token);
			tks = ntks;
		}
		else if ((tok->type == TK_EXPR || tok->type == TK_VALUE) \
		&& (ntok->type == TK_EXPR || ntok->type == TK_VALUE))
		{
			if (prs_join_exprs(tks) < 0)
				return (-1);
		}
		else if (tok->type == TK_VALUE)
		{
			tok->type = TK_EXPR;
		}
		else
			tks = tks->next;
	}
	return (0);
}

char			**prs_argsl(t_dlist *tokens, ENV *envs)
{
	char		**args;
	size_t		argslen;
	size_t		i;
	t_dlist		*it;
	t_tok		*tok;

	argslen = 0;
	prs_vars_derefs(tokens, envs); // TODO: Malloc checking in prs_args.
	prs_values_joins(tokens);
	it = tokens;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		tok->type == TK_EXPR ? ++argslen : 0;
		it = it->next;
	}
	if (!(args = ft_memalloc(sizeof(char*) * (argslen + 1))))
		return (0); // ERROR: prs_args: Malloc failed.
	it = tokens;
	i = -1;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type == TK_EXPR)
			if (!(args[++i] = ft_strdup(tok->value))) // FEATURE: prs_args: Возможное место для обхода экранирования.
				return (0); // ERROR: prs_args: Malloc failed. (LEAKS alert).
		it = it->next;
	}
	return (args);
}
*/

//** Пропуск токенов соответующим флагам (FLAGS & ТИП_ТОКЕНА) == true
t_dlist			*arg_tok_skip(t_dlist *tokens, t_tk_type flags)
{
	while (((t_tok *)tokens->content)->type & flags)
		tokens = tokens->next;
	return (tokens);
}

//** Подсчет количества аргс
size_t			args_count(t_dlist *tokens)
{
	t_tok		*tok;

	tok = tokens->content;
	if (((t_tok *)tokens->content)->type & (TK_EXPR | TK_DEREF | TK_MATH))
	{
		while (0 == (((t_tok *)tokens->content)->type \
		& (TK_SEPS | TK_EMPTY | TK_FLOWS)))
			tokens = tokens->next;
	return (1 + args_count(tokens));
	}
	else if (((t_tok *)tokens->content)->type & (TK_SEPS | TK_FLOWS))
		return (0);
	else
		return (args_count(arg_tok_skip(tokens, TK_FDS_RDS | TK_EMPTY)));
}

char			*get_deref_name(char *key, ENV *envr)
{
	DSTRING		*dstr;
	char		*res;

	if (!(dstr = env_get_variable(key, envr)))
		return (0);
	res = ft_strdup(dstr->txt);
	dstr_del(&dstr);
	if (!res)
		return (0);
	return (res);
}

char			*get_deref_math(char *expr, ENV *envr)
{
	long		num;
	char		*res;
	char		**exprs;
	int			i;

	i = 0;
	exprs = ft_strsplit(expr, ',');
	if (!exprs)
		return (0);
	while (exprs[i])
		num = do_math_bltn(exprs[i++], envr);
	res = ft_lltoa_base(num, 10);
	et_rm_warr(exprs);
	if (!res)
		return (0);
	return (res);
}

//** ПОЛУЧЕНИЕ РАЗЫМЕНОВАНИЙ
char			*get_deref(t_dlist *tokens, ENV *envr)
{
	const t_tok	*tok = tokens->content;

	if (tok->type == TK_NAME)
		return (get_deref_name(tok->value, envr));
	if (tok->type == TK_MATH)
	 	return (get_deref_math(tok->value, envr));
	return (0);
}

//** ПОЛУЧЕНИЕ ARG из не раздельных токенов
t_dlist			*arg_sub(t_dlist *tokens, char **args, size_t ind, ENV *envr)
{
	DSTRING		*expr_buff;
	char		*tmp;
	t_tok		*tok;

	expr_buff = dstr_new("");
	tokens = arg_tok_skip(tokens, TK_FDS_RDS | TK_EMPTY);
	while (!((tok = tokens->content)->type & \
	(TK_SEPS | TK_FLOWS | TK_FDS_RDS | TK_EMPTY)))
	{
		if (tok->type == TK_EXPR)
			dstr_insert_str(expr_buff, tok->value, MAX_LL);
		else if (tok->type == TK_DEREF)
		{
			tokens = tokens->next;
			tmp = get_deref(tokens, envr);
			// ERROR: Если ТМП не вернулось, то значит у нас фейл.
			dstr_insert_str(expr_buff, tmp, MAX_LL);
			free(tmp);
		}
		tokens = tokens->next;
	}
	args[ind] = ft_strdup(expr_buff->txt);
	dstr_del(&expr_buff);
	return (tokens);
}

//** Возвращаем массив args
char			**prs_args(t_dlist *tokens, ENV *envs)
{
	char		**args;
	size_t		ind;
	size_t		argscount;

	argscount = args_count(tokens); // * * Подсчёт аргументов командной строки для выражения.
	args = (char **)ft_memalloc(sizeof(char *) *(argscount + 1)); // * * Выделение памяти на нультермированный массив строк.
	ind = -1;
	while(++ind < argscount)
		if (!(tokens = arg_sub(tokens, args, ind, envs)))
		{
			et_rm_warr(args);
			return (0);
		}
	return (args);
}
