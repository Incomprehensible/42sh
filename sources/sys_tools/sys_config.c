/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:56:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/07 11:57:52 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_errors.h"
#include "dstring.h"
#include "env.h"

/*
** %cN - color begin 0 - 7
** %bN - b_color begin 0 - 7
** %CN - color begin 0 - 7
** %BN - b_color begin 0 - 7
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
	char		buff[8];
	DSTRING		*tmp;
	static char	*var = 0;

	ft_bzero(buff, sizeof(buff));
	len = 2;
	ptr = (*pr_src)->txt + ind + 1;
	buff[0] = '\033';
	buff[1] = '[';
	buff[2] = *ptr <= 'Z' ? '1' : '2';
	buff[3] = ';';
	buff[6] = 'm';
	if (!(ptr[1] >= '0' && ptr[1] <= '7' && ++len) && (buff[3] = 'm'))
		buff[2] = '0';
	else if (*ptr == 'b' || *ptr == 'B')
		buff[4] = '4';
	else
		buff[4] = '3';
	buff[5] = ptr[1];
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
		if (pr_src->txt[ind + 1] == 'c' || pr_src->txt[ind + 1] == 'b' || pr_src->txt[ind + 1] == 'C' || pr_src->txt[ind + 1] == 'B')
			put_prompt_color(&pr_src, ind, &offset);
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
	if (!path || !(result = ft_strdup(path->txt)))
		sys_fatal_memerr(sys_get_std_message(E_ALOCF));
	dstr_del(&path);
	return (result);
}
