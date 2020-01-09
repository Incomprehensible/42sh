/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_second_layer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/29 00:04:32 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short			is_token_here(char *start, char *meta)
{
	if (*start && *start == *meta)
	{
		while (*start && *meta && *start == *meta)
		{
			start++;
			meta++;
		}
		if (!(*meta) && (*(start - 1) == ' ' || (!(*start)
		|| is_separator(*start))))
			return (1);
	}
	return (0);
}

static short	dont_count(char *meta)
{
	if (*meta != '@' && *meta != '^' && *meta != '_'
	&& *meta != '.' && *meta != ' ')
		return (0);
	return (1);
}

static short	can_finish(char *meta)
{
	if ((!*meta || *meta == ' ' || *meta == '_'))
		return (1);
	return (0);
}

static char		*bless_func(char *str, size_t *i, char **meta)
{
	char	*tmp;

	tmp = str;
	if ((str = process_reg(str, *meta)))
	{
		if (**meta == '@')
		{
			*i += get_seq(str, *meta);
			str += *i;
		}
		while (tmp++ != str)
			*i = (dont_count(*meta)) ? *i : ++(*i);
	}
	if (str && (**meta == '@' || **meta == '!' || **meta == '.'))
		*meta = get_point(*meta) + 1;
	else
		++(*meta);
	return (str);
}

size_t			layer_parse_two(char *meta, char *str)
{
	size_t	i;

	i = 0;
	while (*str && *meta && (*str == *meta || special_meta(*meta)))
	{
		if (*meta && special_meta(*meta))
		{
			if (!(str = bless_func(str, &i, &meta)))
				return (0);
		}
		else
		{
			str++;
			meta++;
			i++;
		}
		if (can_finish(meta))
			return (i);
	}
	return (*meta ? 0 : i);
}
