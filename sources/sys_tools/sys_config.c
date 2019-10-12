/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:56:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/12 14:12:48 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_errors.h"
#include "dstring.h"
#include "env.h"

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
			return (prm);
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
