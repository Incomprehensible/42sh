/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_jobs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 07:56:57 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 15:51:25 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "bltn.h"
#include "libftprintf.h"

int		jobs_check_str_job(char *job_s, int md)
{
	ssize_t		i;
	int			fl;
	int			state;
	int			ret;

	i = 0;
	fl = 0;
	while (++i < g_jid)
	{
		if (g_ptab[i])
		{
			if (g_ptab[i]->input_line && \
			ft_strstr(g_ptab[i]->input_line, job_s))
			{
				fl = 1;
				state = sys_wait_prg(&g_ptab[i], &ret, 0, \
				WUNTRACED | WNOHANG | WCONTINUED);
				jobs_put_by_mode(i, md);
				sys_del_if_done(i, state);
			}
		}
	}
	if (fl)
		return (0);
	return (1);
}

int		jobs_check_num_job(char *num, int md)
{
	ssize_t		i;
	int			ret;
	int			state;

	ret = 0;
	if (1 > (i = ft_atoll_base(num, 10)) || i >= g_jid)
		return (jobs_check_str_job(num, md));
	state = sys_wait_prg(&g_ptab[i], &ret, 0, WUNTRACED | WNOHANG | WCONTINUED);
	if (!jobs_put_by_mode(i, md))
		return (jobs_check_str_job(num, md));
	sys_del_if_done(i, state);
	return (0);
}

int		bltn_jobs_iterate(char **args, ENV *envr, int ac, int md)
{
	size_t	j;
	int		ret;

	j = ac;
	while (args[j])
	{
		if (ft_str_isnumeric(args[j], 10))
			ret = jobs_check_num_job(args[j], md);
		else
			ret = jobs_check_str_job(args[j], md);
		if (ret)
			no_job_error(args[j], "jobs: ", envr);
		++j;
	}
	return (ret);
}

/*
** 0 -> usual
** 1 -> p flag
** 2 -> l flag
*/

int		jobs_flag_checker(char **argc, int *print_mode)
{
	int	i;

	i = 0;
	if (argc[1] && argc[1][0] == '-')
		while (argc[1][++i])
		{
			if (argc[1][i] == 'l')
				*print_mode = 2;
			else if (argc[1][i] == 'p')
				*print_mode = 1;
			else
			{
				*print_mode = 0;
				return (1);
			}
		}
	if (!argc[1])
		return (0);
	return (2);
}

int		bltn_jobs(char **args, ENV *envr)
{
	ssize_t		i;
	int			state;
	int			mode;
	int			j;
	int			ret;

	i = 0;
	mode = 0;
	j = jobs_flag_checker(args, &mode);
	if (j == 0 || (j == 2 && !args[2]))
		while (++i < g_jid)
		{
			if (!g_ptab[i])
				continue ;
			state = sys_wait_prg(&g_ptab[i], &ret, 0, \
			WUNTRACED | WNOHANG | WCONTINUED);
			jobs_put_by_mode(i, mode);
			sys_del_if_done(i, state);
		}
	else
		bltn_jobs_iterate(args, envr, j, mode);
	return (0);
}
