/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:02:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/22 13:32:52 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

int			exe_redir_do(int fdl, int fdr, char *file, int flags)
{
	if (file && (fdr = sys_file_op(file, 0, flags, (char *)1)) < 0)
		return (fdr);
	if (fdr != fdl)
	{
		if (dup2(fdr, fdl) < 0)
			return (-E_BADFD);
	}
	return (0);
}

void		exe_redir_type(int *file_flag, t_rdtype tp)
{
	*file_flag = tp != r_rdr ? O_CREAT : 0;
	tp == r_rdr ? *file_flag |= O_RDONLY | O_NOCTTY : 0;
	tp == w_rdr ? *file_flag |= O_WRONLY | O_TRUNC | O_NOCTTY : 0;
	tp == rw_rdr ? *file_flag |= O_RDWR : 0;
	tp == a_rdr ? *file_flag |= O_APPEND | O_WRONLY : 0;
}

int			exe_redir_std(REDIRECT *rdr, int fd, int file_flag, ENV *envr)
{
	if (rdr->file && \
	(fd = sys_file_op(rdr->file, envr, file_flag, (char *)1)) < 0)
		return (fd);
	if (dup2(fd, STDOUT_FILENO) < 0 || dup2(fd, STDERR_FILENO) < 0)
		return (-E_BADFD);
	STDOUT_FILENO != fd && fd != STDERR_FILENO ? close(fd) : 0;
	return (0);
}

int			exe_redir_ex(REDIRECT *rdr, ENV *envr)
{
	int				fd;
	int				file_flag;

	if (rdr->type == herd)
	{
		if (exe_rdr_heredocument(rdr) < 0)
			return (sys_perror("Heredocument io failed!", -1, 0));
	}
	else if ((fd = rdr->fdr) >= 0)
	{
		exe_redir_type(&file_flag, rdr->type);
		if (rdr->fdl == -21)
			return (exe_redir_std(rdr, fd, file_flag, envr));
		return (exe_redir_do(rdr->fdl, rdr->fdr, rdr->file, file_flag));
	}
	else if (fd == -42)
		close(rdr->fdl);
	return (0);
}

int			exe_redir_list(t_dlist *lst, ENV *envr)
{
	int			err;

	err = 0;
	while (lst)
	{
		if (!err && (err = exe_redir_ex(lst->content, envr)))
			sys_error_handler(0, -err, 0);
		lst = lst->next;
	}
	return (err);
}
