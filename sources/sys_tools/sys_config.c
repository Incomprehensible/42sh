/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:56:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/15 15:54:58 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_errors.h"
#include "dstring.h"
#include "env.h"

/*
** %cN - color begin 0 - 7
** %CN - b_color begin 0 - 7
** %S - not bold - not not bold
** %B - bold
** %b - not bold
** %h - hostname
** %u - username
** %? - last status
** %t - time			=DUNNO
** %p - path
** %P - directory
** %n - new_line
*/

void	put_prompt_color(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	size_t		len;
	char		*ptr;
	int			fl;
	char		buff[6];
	DSTRING		*tmp;

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
	tmp = dstr_slice_cut(pr_src, ind, ind + len);
	dstr_del(&tmp);
	*ofs = (size_t)dstr_insert_str(*pr_src, (char *)buff, ind);
}

void	put_prompt_color8bit(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	size_t		len;
	char		*ptr;
	int			fl;
	char		buff[15];
	DSTRING		*tmp;

	ft_bzero(buff, sizeof(buff));
	len = 5;
	fl = 0;
	ptr = (*pr_src)->txt + ind + 1;
	ft_strcpy(buff, "\033[02;38;5;CLRm");
	buff[10] = ptr[1] <= '2' && ptr[1] >= '0' ? ptr[1] : ++fl;
	buff[11] = ptr[2] <= '9' && ptr[2] >= '0' && !fl ? ptr[2] : ++fl;
	buff[12] = ptr[3] <= '9' && ptr[3] >= '0' && !fl ? ptr[3] : ++fl;
	if (fl)
	{
		len = 2;
		ft_strcpy(buff, "\033[0m");
	}
	tmp = dstr_slice_cut(pr_src, ind, ind + len);
	dstr_del(&tmp);
	*ofs = (size_t)dstr_insert_str(*pr_src, (char *)buff, ind);
}

void	put_prompt_new_line(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	int		len;
	DSTRING	*tmp;

	len = 2;
	tmp = dstr_slice_cut(pr_src, ind, ind + len);
	dstr_del(&tmp);
	*ofs = (size_t)dstr_insert_str(*pr_src, "\n", ind);
}

void	put_prompt_bolder(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	int		len;
	DSTRING	*tmp;
	char	*ptr;
	char	buff[6];

	len = 2;
	ptr = (*pr_src)->txt + 1;
	if (*ptr == 'b')
		ft_strcpy(buff, "\033[2m");
	else if (*ptr == 'B')
		ft_strcpy(buff, "\033[1m");
	else
		ft_strcpy(buff, "\033[22m");
	tmp = dstr_slice_cut(pr_src, ind, ind + len);
	dstr_del(&tmp);
	*ofs = (size_t)dstr_insert_str(*pr_src, (char *)buff, ind);
}

DSTRING	*parse_promt(DSTRING *pr_src, ENV *envr)
{
	size_t		ind;
	size_t		offset;

	offset = 0;
	while ((ind = dstr_search_ch(pr_src, '%', offset)) != SIZE_T_MAX)
	{
		if (pr_src->txt[ind + 1] == 'c' || pr_src->txt[ind + 1] == 'C')
			put_prompt_color(&pr_src, ind, &offset);
		else if (pr_src->txt[ind + 1] == 'b' || pr_src->txt[ind + 1] == 'B' || pr_src->txt[ind + 1] == 'S')
			put_prompt_bolder(&pr_src, ind, &offset);
		else if (pr_src->txt[ind + 1] == 'n')
			put_prompt_new_line(&pr_src, ind, &offset);
		else if (pr_src->txt[ind + 1] == 'g')
			put_prompt_color8bit(&pr_src, ind, &offset);
		else
			offset = ind + 2;
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
