/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_parse_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/21 21:39:04 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*pull_token(char *str, size_t i)
{
	char	*new;

	new = ft_memdup(str, i + 1);
	new[i] = 0;
	return (new);
}

char	*pull_single(char *str)
{
	if (str && *str == '\n')
		++str;
	return (ft_strdup(str));
}

char	*trim_str(char *str, short br, t_dlist **tok)
{
	str = (br == '"') ? ++str : str;
	if (*str == ' ' || *str == '\t')
		str = parse_empty(str, 0x0, tok);
	return (parse_sep(str, tok, 0));
}

char	*parse_str_block(char *str, t_dlist **tok, t_stx **tree, short br)
{
	size_t	j;
	short	i;

	j = 0;
	i = 0;
	while (str[j] && !(!i && special_case(br, &str[j])))
	{
		if (str[j] == '\\' && (++j))
			i = i ? 0 : 1;
		else if (!i && valid_deref(&str[j]))
		{
			str += can_pull_tk(j, &str[j], tok, br);
			j = 0;
			if (!(str = parse_deref(str, tok, tree, DQUOTS)))
				return (NULL);
		}
		else
		{
			j++;
			i = 0;
		}
	}
	str += can_pull_tk(j, &str[j], tok, br);
	return (trim_str(str, br, tok));
}

size_t	remove_spaces(char *str, size_t len)
{
	size_t spaces;

	spaces = 0;
	while (*str && len && ft_isspace(*str))
	{
		str--;
		spaces++;
		if (*str == '\\')
		{
			spaces--;
			break ;
		}
		len--;
	}
	return (spaces);
}
