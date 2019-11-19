/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:56:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 08:38:47 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_errors.h"
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

void	put_prompt_bolder(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	int		len;
	char	*ptr;
	char	buff[6];

	len = 2;
	ptr = (*pr_src)->txt + ind + 1;
	if (*ptr == 'b')
		ft_strcpy(buff, "\033[2m");
	else if (*ptr == 'B')
		ft_strcpy(buff, "\033[1m");
	else
		ft_strcpy(buff, "\033[22m");
	dstr_slice_del(pr_src, ind, ind + len);
	*ofs = (size_t)dstr_insert_str(*pr_src, (char *)buff, ind);
}

void	put_prompt_sys_path_uh(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	char			info[PROMPT_MAX_INFO_BUFF];
	char			*ptr;
	int				err;

	err = 0;
	ptr = (*pr_src)->txt + ind + 1;
	if ((*ptr == 'h' || *ptr == 'H') && (ptr = info))
		err = gethostname(info, PROMPT_MAX_INFO_BUFF);
	if (*ptr == 'u' && (ptr = info))
		err = getlogin_r(info, PROMPT_MAX_INFO_BUFF);
	if ((*ptr == 'p' || *ptr == 'P') && (ptr = info))
		err = getcwd(info, PROMPT_MAX_INFO_BUFF) ? 0 : 1;
	if ((*pr_src)->txt[ind + 1] == 'H' && !err)
	{
		if ((ptr = ft_strchr(info, '.')))
			(*ptr) = '\0';
		ptr = info;
	}
	if ((*pr_src)->txt[ind + 1] == 'P' && !err)
		ptr = ft_basename(info);
	dstr_slice_del(pr_src, ind, ind + 2);
	!err ? *ofs = (size_t)dstr_insert_str(*pr_src, ptr, ind) : 0;
	ptr != info ? free(ptr) : 0;
}

char	*prompt_get_status(ENV *env)
{
	DSTRING	*buff;
	char	*ret;

	buff = env_get_variable("?", env);
	if (buff->strlen == 0 || (0 == ft_atoll_base(buff->txt, 10)))
		ret = ft_strdup(PROMPT_OK_STATUS_C);
	else
		ret = ft_strdup(PROMPT_KO_STATUS_C);
	dstr_del(&buff);
	return (ret);
}

char	*prompt_get_time(void)
{
	time_t		timer;
	char		*str;
	char		*buff;

	timer = time(NULL);
	str = ft_strdup("HH:MM");
	if (!str)
		return (0);
	buff = ctime(&timer);
	str[0] = buff[11];
	str[1] = buff[12];
	str[3] = buff[14];
	str[4] = buff[15];
	return (str);
}

void	put_prompt_time_status(DSTRING **pr, size_t ind, size_t *ofs, ENV *env)
{
	char			*info;
	char			*ptr;

	ptr = (*pr)->txt + ind + 1;
	if (*ptr == 't')
		info = prompt_get_time();
	else
		info = prompt_get_status(env);
	dstr_slice_del(pr, ind, ind + 2);
	*ofs = (size_t)dstr_insert_str(*pr, info, ind);
	free(info);
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

DSTRING	*sys_get_prompt_num(ENV *envr, char type)
{
	DSTRING		*prm;
	char		*sprompt;

	sprompt = 0;
	if (type == 0)
	{
		if (!(prm = env_get_variable(SH_VAR_PROMPT, envr)))
			sys_fatal_memerr("PROMT ALLOC FAILED");
		if (prm->strlen != 0)
			return (parse_promt(prm, envr));
		else
			dstr_del(&prm);
	}
	sprompt = type == 'a' ? SH_PROMPT_AP : sprompt;
	sprompt = type == 'q' ? SH_PROMPT_QT : sprompt;
	sprompt = type == 'h' ? SH_PROMPT_HD : sprompt;
	sprompt = type == 's' ? SH_PROMPT_SBH : sprompt;
	sprompt = type == 'l' ? SH_PROMPT_LM : sprompt;
	sprompt = type == 'p' ? SH_PROMPT_PIP : sprompt;
	sprompt = type == '&' ? SH_PROMPT_AND : sprompt;
	sprompt = type == '|' ? SH_PROMPT_OR : sprompt;
	sprompt = sprompt == 0 ? SH_PROMPT : sprompt;
	if (!(prm = dstr_new(sprompt)))
		sys_fatal_memerr("PROMT ALLOC FAILED");
	return (prm);
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
