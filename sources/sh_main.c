/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/18 17:49:38 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "sh_readline.h"
#include "sh_vars.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


int				sh_tokenizer(char *str, t_dlist **token_list);
int				sh_tparser(t_dlist **token_list);

void		write_history(DSTRING *line)
{
	int		fd;

	if (line->strlen == 0 || ft_strcmp(line->txt, "exit()") == 0 \
			|| ft_strcmp(line->txt, "\n") == 0)
		return ;
	dstr_insert_ch(line, '\n', line->strlen);
	if ((fd = open(HISTORY_PATH, O_CREAT | O_EXCL, S_IREAD | S_IWRITE)) != -1)
		close(fd);
	else if ((fd = open(HISTORY_PATH, O_RDWR | O_APPEND)) == -1)
	{
		perror("\nopen failed on history command file");
        exit	(1);
	}
	write(fd, line->txt, line->strlen);
	close(fd);
}

static void		sh_loop(t_envp *env)
{
	DSTRING		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;

	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		line = sh_readline(env);
		write_history(line);
		if (line->txt && ft_strcmp(line->txt, "exit()") == 0)
		{
			dstr_del(&line);
			break ;
		}
		dstr_del(&line);
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
	ft_avl_tree_free(env.global);
	ft_avl_tree_free(env.local);
	return (0);
}

// ls Libf[a-z]/*s/ft?a*