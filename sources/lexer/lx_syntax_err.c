/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_syntax_err.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/25 16:29:53 by bomanyte         ###   ########.fr       */
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

char	*get_err_messg(t_tk_type type)
{
	char	*str;

	str = NULL;
	str = (type == TK_IF) ? ft_strdup("syntax error or unexpected token near 'if' token") : str;
	str = (type == TK_THEN) ? ft_strdup("syntax error or unexpected token near 'then' token") : str;
	str = (type == TK_ELSE) ? ft_strdup("syntax error or unexpected token near 'else' token") : str;
	str = (type == TK_FI) ? ft_strdup("syntax error or unexpected token near 'fi' token") : str;
	str = (type == TK_WHILE) ? ft_strdup("syntax error or unexpected token near 'while' token") : str;
	str = (type == TK_DO) ? ft_strdup("syntax error or unexpected token near 'do' token") : str;
	str = (type == TK_DONE) ? ft_strdup("syntax error or unexpected token near 'done' token") : str;
	str = (type == TK_FOR) ? ft_strdup("syntax error or unexpected token near 'for' token") : str;
	str = (type == TK_IN) ? ft_strdup("syntax error or unexpected token near 'in' token") : str;
	str = (type == TK_BREAK) ? ft_strdup("syntax error or unexpected token near 'break' token") : str;
	str = (type == TK_CONTIN) ? ft_strdup("syntax error or unexpected token near 'continue' token") : str;
	str = (type == TK_UNTIL) ? ft_strdup("syntax error or unexpected token near 'until' token") : str;
	str = (type == TK_EXEC) ? ft_strdup("syntax error or unexpected token near 'exec' token") : str;
	str = (type == TK_FUNCTION) ? ft_strdup("syntax error or unexpected token near 'function' token") : str;
	str = (type == TK_LAMBDA) ? ft_strdup("syntax error or unexpected token near 'lambda' token") : str;
	str = (type == TK_RETURN) ? ft_strdup("syntax error or unexpected token near 'return' token") : str;
	str = (type == TK_FEND) ? ft_strdup("syntax error or unexpected token near the end of function") : str;
	str = (type == TK_ARSHLOCH) ? ft_strdup("syntax error or unexpected token near '&' token") : str;
	str = (type == TK_HERED) ? ft_strdup("syntax error or unexpected token near '<<' token") : str;
	str = (type == TK_OR) ? ft_strdup("syntax error or unexpected token near 'or' token") : str;
	str = (type == TK_AND) ? ft_strdup("syntax error or unexpected token near 'and' token") : str;
	str = (type == TK_RD_W) ? ft_strdup("syntax error or unexpected token near '<' token") : str;
	str = (type == TK_RD_R) ? ft_strdup("syntax error or unexpected token near '>' token") : str;
	str = (type == TK_RD_A) ? ft_strdup("syntax error or unexpected token near '>>' token") : str;
	str = (type == TK_RD_RW) ? ft_strdup("syntax error or unexpected token near '<>' token") : str;
	str = (type == TK_PIPE) ? ft_strdup("syntax error or unexpected token near '|' token") : str;
	str = (type == TK_DEREF) ? ft_strdup("syntax error or unexpected token near '$' token") : str;
	return (str);
}

short	unexpected_token(t_dlist **tok)
{
	t_dlist *token_list;
	char	*str;
	char	*tmp;

	token_list = tok[1];
	while (token_list && TOK_TYPE == TK_EMPTY)
		token_list = token_list->prev;
	if (!token_list)
		str = "syntax error or unexpected token in the beginning of a command";
	if (!TOK_VALUE)
		str = get_err_messg(TOK_TYPE);
	else
	{
		if (*TOK_VALUE == '\n')
			str = ft_strdup("syntax error or unexpected token near new line");
		else
		{
			tmp = ft_strjoin("syntax error or unexpected token near '", TOK_VALUE);
			str = ft_strjoin(tmp, "'");
			free(tmp);
		}
	}
	sys_perror(str, 0, NULL);
	free(str);
	return (-1);
}