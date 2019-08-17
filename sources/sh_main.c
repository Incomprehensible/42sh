/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/17 18:31:14 by fnancy           ###   ########.fr       */
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
	t_dlist *tokens = 0;;

	t_tok	token;

	token.type = cmd_tk;
	token.value = ft_strdup("/bin/echo");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = cmd_tk;
	token.value = ft_strdup("huisadasdasdsd");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));
	
	token.type = rda_tk;
	token.value = 0;
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = filename_tk;
	token.value = ft_strdup("huinya.txt");
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	token.type = eof_tk;
	token.value = 0;
	ft_dlstpush(&tokens, ft_dlstnew(&token, sizeof(t_tok)));

	EXTAB	*tab = parser_get_from_tklist(tokens);

	exe_execute_pi(tab->ex, envp);

	// TERMINATE
	return (0);
}
