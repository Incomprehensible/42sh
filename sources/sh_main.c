/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:25:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/04 12:45:33 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "sh_readline.h"
#include "env.h"
#include "executer.h"
#include "dstring.h"
#include "bltn.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"

#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "sh_tokenizer.h"

// !! TEMPORARY FUNCTIONS ===================================================================== !!
// !! Soon will be changed! =================================================================== !!

#define UT_TOK_INIT()		t_dlist *tokens = 0;\
							t_tok	token
#define UT_TOK_CR(typ, str)	token.type = typ; \
							if (str == 0) token.value = 0; \
							else token.value = ft_strdup(str); \
							ft_dlstpush(&tokens, \
							ft_dlstnew(&token, sizeof(t_tok)))
#define UT_TOK_END()		ft_dlst_delf(&tokens, (size_t)-1, free_token)
#define UT_TOK				tokens

int				bltn_echo(char **args, ENV *envr)
{
	size_t		i;

	i = 0;
	while (args[++i])
	{
		i != 1 ? ft_putstr(" ") : 0;
		ft_putstr(args[i]);
	}
	ft_putstr(" @\n");
	return (0);
}

void			DBG_PRINT_TOKENS(t_dlist *toklst)
{
	t_tok		*token;
	char		*msg;
	char		*value;

	while (toklst)
	{
		token = toklst->content;
		msg = token->type == TK_EMPTY ? "-" : msg;
		msg = token->type == TK_EXPR ? "XP" : msg;
		msg = token->type == TK_SEP ? "SP" : msg;
		msg = token->type == TK_OR ? "OR" : msg;
		msg = token->type == TK_AND ? "AND" : msg;
		msg = token->type == TK_BCKR_PS ? "BGR" : msg;
		msg = token->type == TK_RD_W ? "rW" : msg;
		msg = token->type == TK_RD_R ? "rR" : msg;
		msg = token->type == TK_RD_A ? "rA" : msg;
		msg = token->type == TK_RD_RW ? "rRW" : msg;
		msg = token->type == TK_HERED ? "HRD" : msg;
		msg = token->type == TK_WORD ? "WRD" : msg;
		msg = token->type == TK_FD ? "FD" : msg;
		msg = token->type == TK_FILENAME ? "FLN" : msg;
		msg = token->type == TK_PIPE ? "PIPE" : msg;
		msg = token->type == TK_ASSIGM ? "ASG" : msg;
		msg = token->type == TK_NAME ? "NAME" : msg;
		msg = token->type == TK_VALUE ? "VAL" : msg;
		msg = token->type == TK_MATH ? "MATH" : msg;
		msg = token->type == TK_SUBSH ? "SBSH" : msg;
		msg = token->type == TK_DEREF ? "DRF" : msg;
		msg = token->type == TK_IF ? "IF" : msg;
		msg = token->type == TK_THEN ? "THEN" : msg;
		msg = token->type == TK_ELSE ? "ELSE" : msg;
		msg = token->type == TK_FI ? "FI" : msg;
		msg = token->type == TK_WHILE ? "WHL" : msg;
		msg = token->type == TK_DO ? "DO" : msg;
		msg = token->type == TK_DONE ? "DONE" : msg;
		msg = token->type == TK_FOR ? "FOR" : msg;
		msg = token->type == TK_IN ? "IN" : msg;
		msg = token->type == TK_BREAK ? "BRK" : msg;
		msg = token->type == TK_CONTIN ? "CNT" : msg;
		msg = token->type == TK_EXEC ? "eXC" : msg;
		msg = token->type == TK_FUNCTION ? "FUNC" : msg;
		msg = token->type == TK_LAMBDA ? "LMD" : msg;
		msg = token->type == TK_DQUOTE ? "DQ" : msg;
		msg = token->type == TK_EOF ? "EOF" : msg;
		msg = token->type == TK_FEND ? "FND" : msg;
		msg = token->type == TK_VAR ? "VAR" : msg;
		msg = token->type == TK_UNTIL ? "UTL" : msg;
		msg = token->type == TK_PROC_IN ? "PSIN" : msg;
		msg = token->type == TK_PROC_OUT ? "PSOU" : msg;
		msg = token->type == TK_PROF_IN ? "PFIN" : msg;
		msg = token->type == TK_PROF_OUT ? "PFOU" : msg;
		value = token->value ? token->value : "<->";
		// printf("%5s: %s\n", msg, value);
		toklst = toklst->next;
	}
}

// !! TEMPORARY FUNCTIONS ===================================================================== !!
// !! Soon will be changed! =================================================================== !!

void			add_buf_history(DSTRING *buf)
{
	int			ind;

	ind = S_DARR_STRINGS - (g_histr.count + 1);
	if (ind > 0)
	{
		if (buf->strlen && !(ft_strequ(buf->txt, "exit")))
		{
			g_histr.strings[ind] = dstr_nerr(buf->txt);
			g_histr.count++;
			g_histr.allsize += buf->strlen;
		}
	}
}

void			save_histr(ENV *envr)
{
	int		fd;
	int		ind;
	size_t	count;

	if (!g_histr.count || ((fd = get_history_fd \
	(O_WRONLY | O_TRUNC, "REWRITE_HISTORY: File error", envr)) < 0))
		return ;
	if (g_histr.count > 1000)
		ind = S_DARR_STRINGS - (g_histr.count - 1000);
	else
		ind = S_DARR_STRINGS - 1;
	count = 0;
	while (count <= 1001 && g_histr.strings[ind])
	{
		printf("i = %d\n", ind);
		write(fd, g_histr.strings[ind]->txt, g_histr.strings[ind]->strlen);
		write(fd, "\n", 1);
		ind--;
		count++;
	}
	close(fd);
	// free_darr_re(g_histr.strings, g_histr.count);
}

void			del_history_buf(t_darr *histr)
{
	int		i;
	size_t	count;	

	count = 0;
	i = S_DARR_STRINGS - histr->count - 1;
	while (count < histr->count)
	{
		++count;
		dstr_del(&(histr->strings[i--]));
	}
}

static void		sh_loop(ENV *env)
{
	DSTRING		*line;
	t_dlist		*token_list[2]; // [0] - begining of a tlist, [1] - end;
	int			status;
	DSTRING		*prompt = dstr_new("\033[31;42;1;4mprompt\033[0m ");
	ft_bzero(token_list, sizeof(t_dlist *) * 2);
	while (1)
	{
		init_histr(env);
		line = sh_new_redline(prompt, env);
		add_buf_history(line);
		printf("%s", line->txt);
		save_histr(env);
		dstr_del(&prebuf);
		del_history_buf(&g_histr);
		if (sh_tokenizer(line->txt, token_list) <= 0)
		{
		    dstr_del(&line);
            continue;
        }
		DBG_PRINT_TOKENS(token_list[0]);
		// ft_putendl("== go to parsing ==");
		sh_tparse(token_list[0], env, TK_EOF, &status);
		ft_dlst_delf(token_list, 0, free_token);
		dstr_del(&line);
	}
}

int				main(const int argc, char **argv, char **envp)
{
	ENV			env;
	int			status;
	DSTRING		*dstr;
	
	env_init(argc, argv, envp, &env);
	sys_var_init(&env, argv, argc);
	sys_init();

	ft_avl_set(env.builtns, ft_avl_node_cc("echo", &bltn_echo, 8));

	sh_loop(&env);
	et_rm_clear_env(&env);
	return (0);
}


// #include <fcntl.h>

// int main()
// {
// 	char	name[] = "/Users/gdaemoni/Desktop/test_histr.txt";
// 	int		fd;
// 	int		num = 0;

// 	fd = open(name, O_RDWR | O_CREAT);

// 	while (num < 2500)
// 	{
// 		char *tmp = ft_itoa(num);
// 		write(fd, tmp, ft_strlen(tmp));
// 		write(fd, "\n", 1);
// 		free(tmp);
// 		num++;
// 	}
// 	return (1);
// }