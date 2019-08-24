/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/24 09:02:16 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "executer.h"
#include "dstring.h"

#include <stdio.h>

char			*sh_readline(void);
int				sh_tokenizer(char *str, t_dlist **token_list);
int				sh_tparser(t_dlist **token_list);

static void		sh_loop(void)
{
	char		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		//line = sh_readline();
		//sh_tokenizer(line, token_list);
		//sh_tparser(token_list);
		break ;
	}
}

void			free_token(void *tok, size_t sz)
{
	t_tok	*token;

	token = tok;
	sz = 0;
	if (token->value)
		free(token->value);
	free(token);
}

int				main(const int argc, char **argv, char **envp)
{
	t_dlist *tokens = 0;

	t_tok	token;

/*
	token.type = expr_tk;
	token.value = ft_strdup("/bin/echo");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = expr_tk;
	token.value = ft_strdup("huisadasdasdsd");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = pipe_tk;
	token.value = 0;
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = expr_tk;
	token.value = ft_strdup("/bin/cat");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = expr_tk;
	token.value = ft_strdup("-e");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = eof_tk;
	token.value = 0;
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));
	*/

	token.type = expr_tk;
	token.value = ft_strdup("/bin/sleep");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = expr_tk;
	token.value = ft_strdup("3");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = pipe_tk;
	token.value = 0;
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = expr_tk;
	token.value = ft_strdup("/usr/bin/false");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = eof_tk;
	token.value = 0;
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	sh_tparse(tokens, envp);

	ft_dlst_delf(&tokens, (size_t)-1, free_token);

	// TERMINATE
	return (0);
}
