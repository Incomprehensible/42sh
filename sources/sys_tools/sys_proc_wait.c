/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_proc_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 11:07:06 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/18 21:41:47 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"

#include "stdio.h"

int			sys_wait_cp(pid_t child_pid, int *status)
{
	int		statloc;
	int		signal;
	pid_t	pid;

	signal = 0;
	pid = waitpid(child_pid, &statloc, WUNTRACED);
	if (pid > 0)
	{
		if (WIFEXITED(statloc))
			*status = WEXITSTATUS(statloc);
		else if (WIFSTOPPED(statloc))
			signal = WSTOPSIG(statloc);
		else if (WIFSIGNALED(statloc))
			signal = WTERMSIG(statloc);
	}
	return (signal);
}

int			sys_wait_cps(pid_t last_child, int *status)
{
	int		statloc;
	int		signal;
	pid_t	pid;

	signal = 0;
	while ((pid = waitpid(-1, &statloc, WUNTRACED)) > 0)
	{
		if (pid == last_child)
		{
			if (WIFEXITED(statloc))
				*status = WEXITSTATUS(statloc);
			else if (WIFSTOPPED(statloc))
				signal = WSTOPSIG(statloc);
			else if (WIFSIGNALED(statloc))
				signal = WTERMSIG(statloc);
		}
	}
	return (signal);
}
