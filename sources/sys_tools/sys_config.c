/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:56:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 08:27:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "dstring.h"
#include "parser.h"
#include <time.h>
#include "env.h"

/*
** %cN - color begin 0 - 7
** %CN - b_color begin 0 - 7
** %S - not bold - not dim
** %B - bold
** %b - dim
** %g - 256bit color
** %G - 256bit b_color
** %n - new_line
**
** %h - hostname
** %H - domainname
** %u - username
** %p - path
** %P - directory
**
** %? - last status
** %t - time			=DUNNO
*/

void	put_prompt_color(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	size_t		len;
	char		*ptr;
	int			fl;
	char		buff[6];

	ft_bzero(buff, sizeof(buff));
	len = 3;
	fl = 0;
	ptr = (*pr_src)->txt + ind + 1;
	if (ptr[0] > 'z')
		ft_strcpy(buff, "\033[30m");
	else
		ft_strcpy(buff, "\033[40m");
	ft_strcpy(buff, "\033[30m");
	if ((ptr[1] > '7' || ptr[1] < '0') && --len)
		ft_strcpy(buff, "\033[0m");
	else
		buff[3] = ptr[1];
	dstr_slice_del(pr_src, ind, ind + len);
	*ofs = (size_t)dstr_insert_str(*pr_src, (char *)buff, ind);
}

void	put_prompt_color8bit(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	size_t		len;
	char		*ptr;
	int			fl;
	char		buff[12];

	ft_bzero(buff, sizeof(buff));
	len = 5;
	fl = 0;
	ptr = (*pr_src)->txt + ind + 1;
	ft_strcpy(buff, "\033[b8;5;CLRm");
	buff[2] = ptr[0] == 'g' ? '3' : '4';
	buff[7] = ptr[1] <= '2' && ptr[1] >= '0' ? ptr[1] : ++fl;
	buff[8] = ptr[2] <= '9' && ptr[2] >= '0' && !fl ? ptr[2] : ++fl;
	buff[9] = ptr[3] <= '9' && ptr[3] >= '0' && !fl ? ptr[3] : ++fl;
	if (fl)
	{
		len = 2;
		ft_strcpy(buff, "\033[0m");
	}
	dstr_slice_del(pr_src, ind, ind + len);
	*ofs = (size_t)dstr_insert_str(*pr_src, (char *)buff, ind);
}

void	put_prompt_new_line(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	int		len;

	len = 2;
	dstr_slice_del(pr_src, ind, ind + len);
	*ofs = (size_t)dstr_insert_str(*pr_src, "\n", ind);
}

DSTRING	*parse_promt(DSTRING *pr_src, ENV *envr)
{
	size_t		ind;
	size_t		offset;
	char		sp;

	offset = 0;
	while ((ind = dstr_search_ch(pr_src, '%', offset)) != SIZE_T_MAX)
	{
		sp = pr_src->txt[ind + 1];
		if (sp == 'c' || sp == 'C')
			put_prompt_color(&pr_src, ind, &offset);
		else if (sp == 'b' || sp == 'B' || sp == 'S')
			put_prompt_bolder(&pr_src, ind, &offset);
		else if (sp == 'n')
			put_prompt_new_line(&pr_src, ind, &offset);
		else if (sp == 'g' || sp == 'G')
			put_prompt_color8bit(&pr_src, ind, &offset);
		else if (sp == 'h' || sp == 'u' || sp == 'p' || sp == 'P' || sp == 'H')
			put_prompt_sys_path_uh(&pr_src, ind, &offset);
		else if (sp == 't' || sp == '?')
			put_prompt_time_status(&pr_src, ind, &offset, envr);
		else
			dstr_slice_del(&pr_src, ind, (offset = ind + 1));
	}
	return (pr_src);
}

char	*sys_get_conf_path(char *file, ENV *envr)
{
	DSTRING		*path;
	char		*result;

	if (!(path = env_get_variable(SH_VAR_HIST_RC_PATH, envr)))
		sys_fatal_memerr(sys_get_std_message(E_ALOCF));
	dstr_insert_ch(path, '/', MAX_LL);
	dstr_insert_str(path, file, MAX_LL);
	result = ft_strdup(path->txt);
	if (!path || !(result))
		sys_fatal_memerr(sys_get_std_message(E_ALOCF));
	dstr_del(&path);
	return (result);
}
