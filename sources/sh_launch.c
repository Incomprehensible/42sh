/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 04:39:52 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/13 04:05:06 by hgranule         ###   ########.fr       */
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
#include "ft_lbuffer.h"

#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "sh_tokenizer.h"

int			sh_launch_string(ENV *env, char *string)
{
	int			status;
	t_dlist		*toks[2];
	int			wtf;

	if (!string)
		sys_fatal_memerr("STRING_EX_FAIL");
	ft_bzero(toks, sizeof(t_dlist *) * 2);
	if ((wtf = sh_tokenizer(string, toks)) <= 0)
	{
		sys_error_message("ERROR FILE END", 0);
	}
	else
		sh_tparse(toks[0], env, TK_EOF, &status);
	if (toks[0])
		ft_dlst_delf(toks, 0, free_token);
	return (0);
}

int			sh_launch_file(ENV *env, char *file)
{
	int			fd;
	t_lbuf		*buffer;
	char		*file_cont;

	file_cont = 0;
	if (!(SYS_TCH_F & sys_touch_file(file)))
		return (sys_error_handler(0, E_FOPEF, env));
	if ((fd = sys_file_op(file, env, O_RDONLY, 0)) < 0)
		return (fd);
	if (!(buffer = ft_lb_readbytes(fd, 0)))
		sys_fatal_memerr("FILE_READ_FAIL");
	close(fd);
	if (!(file_cont = ft_lb_flush(buffer)))
		sys_fatal_memerr("FILE_READ_FAIL");
	sh_launch_string(env, file_cont);
	free(file_cont);
	return (0);
}

int			sys_core_set_init(ENV *env, char **argv)
{
	int		i;
	int		j;
	char	*key;
	char	*argc_val;

	i = 0;
	j = 0;
	while (argv[++i])
	{
		if (argv[i][0] == '-')
			continue ;
		key = ft_itoa(j);
		if (!key || env_core_set((char *)key, argv[i], env->core) < 0)
			sys_fatal_memerr("MALLOC CALL FAILED, CORE VARIABLES LOST!");
		free(key);
		++j;
	}
	if (!(argc_val = ft_itoa(j)) || \
	(env_core_set("#", argc_val, env->core) < 0))
		sys_fatal_memerr("MALLOC CALL FAILED, CORE VARIABLES LOST!");
	free(argc_val);
	return (0);
}

char		*get_file_from_arg(char **argv, char ***afile)
{
	char	*file;
	size_t	i;

	file = 0;
	i = 0;
	while (argv[++i])
	{
		if (argv[i][0] == '-')
			continue ;
		*afile = &argv[i];
		file = argv[i];
		break ;
	}
	return (file);
}

// TEMP: TODO BETTER
int			sh_launch_system(ENV *env, char **lflags, \
char *flags, char **argv)
{
	char	*file;
	char	**afile;

	file = get_file_from_arg(argv, &afile);
	if (flags && ft_strchr(flags, 'c') && file)
	{
		sys_core_set_init(env, afile);
		sh_launch_string(env, file);
		if (!flags || (flags && !ft_strchr(flags, 'r')))
			return (0);
	}
	else if (file)
	{
		sys_core_set_init(env, argv);
		sh_launch_file(env, file);
		if (!flags || (flags && !ft_strchr(flags, 'r')))
			return (0);
	}
	else if (!file)
	{
		env_core_set("0", argv[0], env->core);
		env_core_set("#", "0", env->core);
	}
	sh_loop(env);
	return (0);
}
