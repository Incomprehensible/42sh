/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:17:54 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/13 14:31:16 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

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
		if (tok->type == TK_EXPR || tok->type == TK_VALUE)
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
