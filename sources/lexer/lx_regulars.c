/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_regulars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/22 01:25:46 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*ft_process_limits(char *str)
{
	char	limit;

	if (*str && (*str == '\'' || *str == '"' || *str == '`'))
		limit = *str++;
	else
		limit = ' ';
	while (*str && !is_sep_no_space(*str) && *str != limit)
		str++;
	return (str - 1);
}

char	*ft_process_ignore(char *str, char *meta)
{
	meta++;
	while (*str && !is_sep_no_space(*str) && !(special_meta(*meta)))
	{
		if (*str != *meta)
			return (0);
		str++;
		meta++;
	}
	return (str);
}

char	*process_reg(char *str, char *meta)
{
	if (*meta == '?')
		str = ft_process_s(str, meta);
	else if (*meta == '^' || *meta == '_')
		str = ft_process_space(str, meta);
	else if (*meta == '~')
		str = ft_process_any(str, meta);
	else if (*meta == '!')
		str = ft_process_wall(str, meta);
	else if (*meta == '.')
		str = ft_process_ignore(str, meta);
	else if (*meta == '%')
		str = ft_process_limits(str);
	else if (*meta == 'z')
		str = ft_process_all(str, meta);
	else if (*meta == 'w')
		str = ft_process_trash(str, meta);
	else
		str = ft_process_seq(str, meta);
	return (str);
}

char	*get_point(char *meta)
{
	if (*meta == '!')
	{
		while (*(meta + 1))
			meta++;
		return (meta);
	}
	else if (*meta == '.')
	{
		++meta;
		while (*meta && !(special_meta(*(meta + 1))))
			meta++;
		return (meta);
	}
	++meta;
	while (*meta && *meta != '@')
		meta++;
	return (meta);
}

char	*reg_process(char *patt, t_tk_type type, char *str, t_dlist **tok)
{
	size_t	i;
	char	*new;

	if (!(*str))
		return (str);
	if ((i = layer_parse_two(patt, str)))
	{
		if (type == TK_FUNCTION)
			new = NULL;
		else
			new = pull_token(str, --i);
		make_token(tok, new, type);
	}
	str += i;
	return (str);
}
