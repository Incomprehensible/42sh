/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_checkfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 16:03:36 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/26 14:54:04 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"
#include <sys/stat.h>
#include <fcntl.h>

static void			t_checkfile_concatpath(DSTRING **path, char *str)
{
	(*path) = dstr_new(g_pwd->txt);
	dstr_insert_ch((*path), '/', (*path)->strlen);
	dstr_insert_str((*path), str, (*path)->strlen);
}

static int			t_checkfile_rwx(struct stat *buf, t_flags_file *fl)
{
	if ((buf->st_mode & S_IRUSR) && fl->f_read)
		return (0);
	else if ((buf->st_mode & S_IWUSR) && fl->f_write)
		return (0);
	else if ((buf->st_mode & S_IXUSR) && fl->f_exec)
		return (0);
	return (1);
}

static int			t_checkfile(struct stat *buf, t_flags_file *fl)
{
	if (S_ISDIR(buf->st_mode) && fl->f_dir)
		return (0);
	else if (S_ISREG(buf->st_mode) && fl->f_file)
		return (0);
	else if (S_ISCHR(buf->st_mode) && fl->f_symbdev)
		return (0);
	else if (S_ISBLK(buf->st_mode) && fl->f_blckdev)
		return (0);
	else if (S_ISFIFO(buf->st_mode) && fl->f_chnl)
		return (0);
	else if (S_ISLNK(buf->st_mode) && fl->f_symb)
		return (0);
	else if (S_ISSOCK(buf->st_mode) && fl->f_sckt)
		return (0);
	else if ((buf->st_mode & S_ISVTX) && fl->f_stbit)
		return (0);
	else if ((buf->st_mode & S_ISUID) && fl->f_suid)
		return (0);
	else if ((buf->st_mode & S_ISGID) && fl->f_segid)
		return (0);
	else if (fl->f_exst)
		return (0);
	else if (fl->f_notnull && buf->st_size != 0)
		return (0);
	return (t_checkfile_rwx(buf, fl));
}

int					bltn_test_checkfile(char **args, t_flags_file *fl)
{
	struct  stat	buf;
	DSTRING			*path;
	int				a;

	if (args[2][0] != '/')
		t_checkfile_concatpath(&path, args[2]);
	else
		path = dstr_new(args[2]);
	// if (fl->fd)
	// {
	// 	if ((a = fcntl(ft_atoi(path->txt), F_GETFL)) == -1)
	// 		return (1);
	// 	else
	// 	{
	// 		dstr_del(&path);
	// 		return (0);			
	// 	}
	// }
	if (lstat(path->txt, &buf) == -1)
	{
		dstr_del(&path);
		return (1);
	}
		dstr_del(&path);
		return (t_checkfile(&buf, fl));
	return (0);	
}