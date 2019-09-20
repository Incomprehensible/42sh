/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_touch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 22:05:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/20 23:04:44 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include <sys/stat.h>

int		sys_touch_file(const char *path)
{
	int				perms;
	struct stat		st;

	perms = 0;
	if (access(path, F_OK) == 0)
		perms |= 8;
	if (access(path, X_OK) == 0)
		perms |= 1;
	if (access(path, W_OK) == 0)
		perms |= 2;
	if (access(path, R_OK) == 0)
		perms |= 4;
	if (!stat(path, &st))
		perms |= (st.st_mode & 00170000 >> 8);
	return (perms);
}
