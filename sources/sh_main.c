/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/22 23:47:17 by hgranule         ###   ########.fr       */
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

int				main(const int argc, char **argv, char **envp)
{
	t_dlist *tokens = 0;

	t_tok	token;

	token.type = expr_tk;
	token.value = ft_strdup("/bin/echo");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = expr_tk;
	token.value = ft_strdup("huisadasdasdsd");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = fd_tk;
	token.value = ft_strdup("1");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = rd_w_tk;
	token.value = 0;
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = filename_tk;
	token.value = ft_strdup("PASHA.TXT");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = eof_tk;
	token.value = 0;
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	sh_tparse(tokens, envp);

	// TERMINATE
	return (0);
}
