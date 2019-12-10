/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_envar_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/10 22:39:34 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static char	*get_value(char *str, t_stx **tr, t_dlist **tok)
{
	char	*new;

	if (*str == '\\' && (*(str + 1) == ' ' || *(str + 1) == '\t'))
		str += 2;
	// if (*str == '\'')
	// 	new = parse_apofs(str, tok, tr, 0);
	//else
		// new = parse_assig_block(str, tok, tr);
	new = parse_assig_block(str, tok, tr);
	if (new != str && new)
		substitute_value(tok[1]);
	if (new == str)
		make_token(tok, NULL, TK_VALUE);
	if (!new)
		return (NULL);
	return (new);
}

static char	*get_assigment(char *str, t_dlist **tok)
{
	make_token(tok, pull_token(str, 1), TK_ASSIGM);
	if (*str == '+' || *str == '-')
	{
		if (*(++str) != '=')
			return (NULL);
	}
	return (++str);
}

static char	*get_envar_name(char *str, t_dlist **tok, short i)
{
	char	*new;

	while (*str && !is_separator(*str) && !is_assign(*str))
	{
		str = (*str == '\\' && *str) ? str + 2 : ++str;
		i++;
	}
	if (!(*str) || is_separator(*str))
		return (NULL);
	new = pull_token(str - i, i);
	if (!validate_envar(new) || !validate_var(new))
	{
		free(new);
		return (NULL);
	}
	make_token(tok, new, TK_VAR);
	return (str);
}

char		*into_envar(char *str, t_dlist **tok, t_stx **tree)
{
	char	*tmp;

	if (!(tmp = get_assigment(str, tok)))
		return (parse_comm(str, tok, tree, 0));
	if (!(str = get_value(tmp, tree, tok)))
		return (NULL);
	if (*str == ' ' || *str == '\t')
		str = parse_empty(str, 0x0, tok);
	str = parse_sep(str, tok, 0);
	if (following_pipe(tok[1]))
		return (NULL);
	return (str);
}

char		*parse_envar(char *str, t_dlist **tok, t_stx **tree, short i)
{
	char *const	patt = "?= ";
	char *const	patt2 = "?+= ";
	char		*tmp;

	str = parse_empty(str, 0x0, tok);
	if (layer_parse_two(patt, str) || layer_parse_two(patt2, str))
	{
		if (following_pipe(tok[1]))
			return (NULL);
		if (!(tmp = get_envar_name(str, tok, 0)))
			return (parse_comm(str, tok, tree, 0));
		str = into_envar(tmp, tok, tree);
	}
	else
		return (parse_comm(str, tok, tree, i));
	return (str);
}
