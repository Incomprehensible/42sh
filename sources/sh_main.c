/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/12 01:52:06 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"

char			*sh_readline(void);
int				sh_tokenizer(char *str, t_dlist **token_list, char **env);
int				sh_tparser(t_dlist **token_list);

static void		sh_loop(char **env)
{
	char		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		//line = sh_readline();
		//sh_tokenizer(line, token_list)
		get_next_line(0, &line);
		if (!sh_tokenizer(line, token_list, env))
		{
		    free(line);
            continue;
        }
		//sh_tparser(token_list);
		free(line);
		//break ;
	}
}

int				main(const int argc, char *const *argv, char *const *envp)
{
	// INIT

	// LOOP
	sh_loop(envp);

	// TERMINATE
	return (0);
}
