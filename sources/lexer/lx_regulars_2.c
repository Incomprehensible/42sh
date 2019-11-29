/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_regulars_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/22 01:03:52 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*ft_process_seq(char *str, char *meta)
{
	char	current;

	++meta;
	while (*meta && *meta != '@')
	{
		current = *meta;
		if (*str == current)
			return (str);
		meta++;
	}
	return (0);
}

char	*ft_process_all(char *str, char *meta)
{
	short	flag;

	flag = 0;
	++meta;
	while (*str && *str != *meta)
	{
		if (*meta == ')' && (*str == '"' || *str == '\''))
			str += skip_field(str, *str);
		if (*str && !is_separator(*str))
			flag = 1;
		if (*str == '\\' && *(str + 1) && (flag = 1))
			str++;
		str++;
	}
	if (*str == *meta && flag)
		return (str);
	return (0);
}

char	*ft_process_any(char *str, char *meta)
{
	++meta;
	while (*str && !is_sep_no_space(*str) && *str != *meta &&
	!(is_meta(str, *meta)))
	{
		str = (*str == '\\') ? str + 2 : ++str;
	}
	if (*str == *meta || (is_meta(str, *meta)))
		return (str);
	return (0);
}

char	*ft_process_vars(t_tk_type type, char *str, char *meta, t_dlist **tok)
{
	size_t	i;

	i = 0;
	++meta;
	while (*str && *str != *meta && *str != *(meta + 2))
	{
		str++;
		i++;
	}
	make_token(tok, pull_token(str - i, i), type);
	return (str);
}

char	*ft_process_wall(char *str, char *meta)
{
	++meta;
	while (*str && !is_sep_no_space(*str) && !is_token_here(str, meta))
		str++;
	if (is_sep_no_space(*str))
		return (NULL);
	if (!(*str))
		return (0);
	return (str);
}
