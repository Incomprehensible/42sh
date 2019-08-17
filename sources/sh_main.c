/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/17 16:06:57 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "sh_readline.h"
#include "sh_vars.h"

int				sh_tokenizer(char *str, t_dlist **token_list);
int				sh_tparser(t_dlist **token_list);

static void		sh_loop(t_envp *env)
{
	char		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		line = sh_readline(env);
		printf("\n%s\n", line);
		free(line);
		// sh_tokenizer(line, token_list);
		// sh_tparser(token_list);
		
		break ;
	}
}

int				main(const int argc, char **argv, char **envp)
{
	t_envp	env;
	// INIT
	sh_init_vars(argc, argv, envp, &env);
	// LOOP
	sh_loop(&env);
	// TERMINATE
	return (0);
}
