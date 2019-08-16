/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/16 12:51:02 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "sh_readline.h"

int				sh_tokenizer(char *str, t_dlist **token_list);
int				sh_tparser(t_dlist **token_list);

int		test_t_darr()
{
	char	*path;
	t_darr	a;
	
	path = ".";
	a = sh_dir_content(path);
	ft_putendl(a.strings[0]->txt);
	ft_putendl(a.strings[1]->txt);
	ft_putendl(a.strings[2]->txt);
	ft_putendl(a.strings[3]->txt);
	ft_putendl(a.strings[4]->txt);
	free_darr(a.strings);
}

static void		sh_loop(void)
{
	char		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		line = sh_readline();
		printf("\n%s\n", line);
		free(line);
		// sh_tokenizer(line, token_list);
		// sh_tparser(token_list);
		
		break ;
	}
}

int				main(const int argc, char *const *argv, char *const *envp)
{
	// INIT

	// LOOP
	sh_loop();

	// TERMINATE
	return (0);
}
