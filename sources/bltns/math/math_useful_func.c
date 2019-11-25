/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_useful_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 02:33:26 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

short		get_operator_tok(t_tk_type *ops, t_tk_type type)
{
	short	i;
	short	j;

	i = 0;
	j = 0;
	while (ops[i * 16])
	{
		while (ops[i * 16 + j])
		{
			if (type == ops[i * 16 + j])
				return (i);
			j++;
		}
		i++;
		j = 0;
	}
	return (0);
}

t_dlist		*go_through_brackets(t_dlist *dimon_loh, t_tk_type type)
{
	while (dimon_loh && (type == OUT_BR || type == TK_EOF))
	{
		dimon_loh = dimon_loh->next;
		type = dimon_loh ? ((t_tok *)dimon_loh->content)->type : 0;
	}
	return (dimon_loh);
}

size_t		get_base_seq(char *str, char *meta)
{
	while (*str)
	{
		if (!got_in_seq(*str, meta + 1))
			return (0);
		str++;
	}
	return (1);
}

static void	make_base_tok(char *op, t_mtx *base, t_dlist **math)
{
	if (base->base == SEV)
	{
		if (base->next)
			make_token(math, ft_strdup(op + 2), base->base);
		else
			make_token(math, ft_strdup(op + 1), base->base);
		free(op);
	}
	else
		make_token(math, op, base->base);
}

t_tk_type	get_base(char *op, t_mtx **bases, t_dlist **math)
{
	t_mtx	*base;
	size_t	i;

	i = 2;
	while (bases[i])
	{
		base = bases[i];
		while (base)
		{
			if (layer_parse_two(base->strt_meta, op))
			{
				if (!(parse_base(base->fin_meta, op)))
					return (0);
				make_base_tok(op, base, math);
				return (base->base);
			}
			base = base->next;
		}
		i++;
	}
	return (0);
}
