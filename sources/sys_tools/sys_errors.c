/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:37:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/20 23:07:10 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_errors.h"

char		*errmess[ERR_CNT] =
{
	"UNDEFINED ERROR",
	"Command not found",
	"Fork call failed",
	"Exec call failed",
	"Pipe call failed",
	"Permissions denied",
	"Redirection denied",
	"Bad file descriptor",
	"Memory allocation failed",
	"Is a directory",
	"Is not a regular file"
};

char			*sys_get_std_message(int ecode)
{
	if (ecode > ERR_CNT)
		ecode = 0;
	return (errmess[ecode]);
}
