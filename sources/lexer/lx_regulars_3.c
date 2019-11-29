/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_regulars_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 00:32:21 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

int				special_meta(char meta)
{
	if (meta == '@' || meta == '^' || meta == '~' || meta == '_'
	|| meta == '!' || meta == '%' || meta == '.'
	|| meta == '?' || meta == 'z' || meta == 'w')
		return (1);
	return (0);
}

short			is_meta(char *str, char meta)
{
	if (meta == ' ' || meta == '^')
	{
		if (meta == ' ' && (*str == ' ' || *str == '\t' ||
		*str == ';' || *str == '\n' || !(*str)))
			return (1);
		if (meta == '^' && ft_process_space(str, &meta))
			return (1);
	}
	return (0);
}

char			*ft_process_s(char *str, char *meta)
{
	if ((!(*str >= 65 && *str <= 90) && !(*str >= 97 && *str <= 122)) &&
	*str != '_' && !(ft_isdigit(*str)))
		return (0);
	while (*str && !is_separator(*str) && *str != *(meta + 1))
	{
		if (is_prefix(*str) || is_ariphmetic(*str))
			return (NULL);
		str = (*str == '\\') ? str + 2 : ++str;
	}
	if (!(*str) && !(is_meta(str, *(meta + 1))))
		return (0);
	return (str);
}

char			*ft_process_space(char *str, char *meta)
{
	if (*meta == '_')
	{
		if (*str == ' ' || *str == '\t' || !(*str) ||
		is_sep_no_space(*str) || *str == '(')
			return (str);
		return (0);
	}
	if (*str != ' ' && *str != '\t' && *str)
		return (0);
	while (*str && str != meta && !is_sep_no_space(*str) &&
	(*str == ' ' || *str == '\t'))
		str++;
	if (*str)
		return (str);
	return (0);
}

char			*ft_process_trash(char *str, char *meta)
{
	++meta;
	while (*str && *str != *meta)
	{
		if (*str == '\\')
			str++;
		str++;
	}
	if (*str == *meta)
		return (str);
	return (0);
}
