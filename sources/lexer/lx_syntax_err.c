/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_syntax_err.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 18:38:44 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"
#include "ft_io.h"
#include "dstring.h"
#include <unistd.h>
#include <termios.h>

char	*get_err_1(t_tk_type type)
{
	char	*str;

	str = (type == TK_HERED) ? cook_err_msg("<<") : NULL;
	str = (type == TK_OR) ? cook_err_msg("or") : str;
	str = (type == TK_AND) ? cook_err_msg("and") : str;
	str = (type == TK_RD_W) ? cook_err_msg("<") : str;
	str = (type == TK_RD_R) ? cook_err_msg(">") : str;
	str = (type == TK_RD_A) ? cook_err_msg(">>") : str;
	str = (type == TK_RD_RW) ? cook_err_msg("<>") : str;
	str = (type == TK_PIPE) ? cook_err_msg("|") : str;
	str = (type == TK_DEREF) ? cook_err_msg("$") : str;
	return (str);
}

char	*get_err_2(t_tk_type type)
{
	char	*str;

	str = (type == TK_IF) ? cook_err_msg("if") : NULL;
	str = (type == TK_THEN) ? cook_err_msg("then") : str;
	str = (type == TK_ELSE) ? cook_err_msg("else") : str;
	str = (type == TK_FI) ? cook_err_msg("fi") : str;
	str = (type == TK_WHILE) ? cook_err_msg("while") : str;
	str = (type == TK_DO) ? cook_err_msg("do") : str;
	str = (type == TK_DONE) ? cook_err_msg("done") : str;
	str = (type == TK_FOR) ? cook_err_msg("for") : str;
	str = (type == TK_IN) ? cook_err_msg("in") : str;
	str = (type == TK_BREAK) ? cook_err_msg("break") : str;
	str = (type == TK_CONTIN) ? cook_err_msg("continue") : str;
	return (str);
}

char	*get_err_messg(t_tk_type type)
{
	char	*str;

	if (type >= TK_HERED && type <= TK_DEREF)
		return (get_err_1(type));
	else if (type >= TK_IF && type <= TK_CONTIN)
		return (get_err_2(type));
	str = (type == TK_UNTIL) ? cook_err_msg("until") : NULL;
	str = (type == TK_EXEC) ? cook_err_msg("exec") : str;
	str = (type == TK_FUNCTION) ? cook_err_msg("function") : str;
	str = (type == TK_LAMBDA) ? cook_err_msg("lambda") : str;
	str = (type == TK_RETURN) ? cook_err_msg("return") : str;
	str = (type == TK_FEND) ? ft_strdup(FENDERR) : str;
	str = (type == TK_ARSHLOCH) ? cook_err_msg("&") : str;
	return (str);
}

char	*defined_error(t_dlist *token_list)
{
	char	*str;
	char	*tmp;

	if (!TOK_VALUE)
		str = get_err_messg(TOK_TYPE);
	else
	{
		if (*TOK_VALUE == '\n')
			str = ft_strdup(NEWLERR);
		else
		{
			tmp = ft_strjoin(SIMPERR, TOK_VALUE);
			str = ft_strjoin(tmp, "'");
			free(tmp);
		}
	}
	return (str);
}

short	unexpected_token(t_dlist **tok)
{
	t_dlist *token_list;
	char	*str;

	token_list = tok[1];
	if (!token_list || !token_list->content)
		str = ft_strdup(STRTERR);
	else
	{
		while (token_list && TOK_TYPE == TK_EMPTY)
			token_list = token_list->prev;
		if (!token_list || !token_list->content)
			str = ft_strdup(STRTERR);
		else
			str = defined_error(token_list);
	}
	sys_perror(str, 0, NULL);
	free(str);
	return (-1);
}
