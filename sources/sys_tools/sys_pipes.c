/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 08:50:28 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/18 09:52:32 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"

static int	sys_pipe_find_actual()
{
	size_t	i;

	i = SYS_FDS_USR_LIMIT;
	while (i < SYS_PIPES_SIZE)
	{
		if (sys_pipes[i] == 0)
		{
			sys_pipes[i] = 1;
			return (i);
		}
		++i;
	}
	return (-1);
}

static int	return_code_err_pip(int *a, int code)
{
	if (a[0] >= 0)
		sys_destroy_pipe(a[0]);
	if (a[1] >= 0)
		sys_destroy_pipe(a[1]);
	return (code);
}

/*
** Creates a pipe after USER_LIMIT_BORDER
*/
int			sys_create_pipe(int	*a)
{
	int		b[2];

	if (pipe(b) < 0)
		return (-1); // PIPES LOOSES
	if (b[0] > SYS_PIPES_SIZE || b[1] > SYS_PIPES_SIZE)
		return (return_code_err_pip(b, -2));
	a[0] = dup2(b[0], sys_pipe_find_actual());
	a[1] = dup2(b[1], sys_pipe_find_actual());
	if (*((unsigned long *)a) & CHECK_INTS_P)
	{
		return_code_err_pip(a, 0);
		return (return_code_err_pip(b, -2));
	}
	close(b[0]);
	close(b[1]);
	sys_pipes[a[0]] = 1;
	sys_pipes[a[1]] = 1;
	return (0);
}

int			sys_destroy_pipe(int p)
{
	if (sys_pipes[p])
		sys_pipes[p] = 0;
	return (close(p));
}

int			sys_kill_pipes(void)
{
	size_t	i;

	i = SYS_FDS_USR_LIMIT;
	while (i < SYS_PIPES_SIZE)
	{
		if (sys_pipes[i])
		{
			sys_pipes[i] = 0;
			if (close(i) < 0)
				return (-1);
		}
		++i;
	}
	return (0);
}
