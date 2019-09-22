/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:56:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 18:12:47 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_errors.h"
#include "dstring.h"
#include "env.h"

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
