/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_heredoc_getline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 18:08:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/17 18:08:27 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

DSTRING		*hered_getline(DSTRING *prm, ENV *env)
{
	DSTRING		*line;
	int			status;

	if (g_input_file >= 0)
	{
		status = get_next_line(g_input_file, &line);
		return (line);
	}
	return (sh_readline(prm, env));
}
