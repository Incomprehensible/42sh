/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_first_layer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 00:23:12 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

size_t		decimals_proceed(char *str, char *meta)
{
	size_t	i;

	i = 0;
	if (*meta == '@')
	{
		if ((str = ft_process_seq(str, meta)))
			i = get_seq(str, meta);
		if (!str || !i)
			return (0);
		str += i;
	}
	meta = (*meta == '@') ? get_point(meta) + 1 : ++meta;
	while (*meta && *str && *meta == *str)
	{
		str++;
		meta++;
		i++;
	}
	if (!(*meta) && *str != '-')
		return (i);
	return (0);
}

int			layer_parse_one(char *meta, char *str)
{
	while (*str && *meta && !(is_sep_no_space(*str) && *meta != 'w') &&
	(*str == *meta || special_meta(*meta)))
	{
		if (*meta && special_meta(*meta))
		{
			if (!(str = process_reg(str, meta)))
				return (0);
			if (*meta == '@' || *meta == '!' || *meta == '.')
				meta = get_point(meta);
			meta++;
		}
		else
		{
			str++;
			meta++;
		}
	}
	if (!*meta || (*meta == '_' && is_sep_no_space(*str)))
		return (1);
	return (0);
}

char		*block_pass(short i, char *str, t_dlist **tok, t_stx **tree)
{
	static char*(*ptr[13])(char*, t_dlist**, t_stx**, short);

	if (!ptr[0])
	{
		ptr[0] = &parse_dquotes;
		ptr[1] = &parse_apofs;
		ptr[2] = &parse_math;
		ptr[3] = &parse_subsh;
		ptr[4] = &parse_scripts;
		ptr[5] = &parse_envar;
		ptr[6] = &parse_hedoc;
		ptr[7] = &parse_proc;
		ptr[8] = &parse_redir;
		ptr[9] = &parse_func;
		ptr[10] = &parse_lambda;
		ptr[11] = &parse_comm;
		ptr[12] = &parse_deref;
	}
	if (i == EMPTY)
		return (parse_empty(str, 0x0, tok));
	else if (i == COMMENT)
		return (skip_comment(str));
	else if (i == SEPS)
		return (parse_sep(str, tok, 0));
	return (ptr[i](str, tok, tree, 0));
}

int			check_branch(char *str, t_stx *tree)
{
	short	choice;

	choice = 0;
	while (!choice && *str && tree)
	{
		choice = layer_parse_one(tree->meta, str);
		tree = tree->next;
	}
	return (choice);
}

short		find_token(t_stx **tree, char *str)
{
	t_tk_type	i;
	short		choice;

	i = 0;
	choice = 0;
	if (ft_isspace(*str) || *str == '\n')
		return (EMPTY);
	else if (*str == '$' && valid_deref(str))
		return (DEREF);
	else if (is_sep_no_space(*str) && *str && !is_redir(str + 1))
		return (SEPS);
	while (*str && !(is_sep_no_space(*str) && !is_redir(str + 1)) && !choice)
	{
		i = 0;
		if (*str == '$' || *str == '\\')
			return (EXPRS);
		if (*str == '#')
			return (COMMENT);
		while (tree[i] && !(choice = check_branch(str, tree[i])))
			i++;
		if (!tree[i])
			str++;
	}
	return (tree[i] ? i : EXPRS);
}
