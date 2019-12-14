/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_siglog.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 19:27:17 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/14 19:41:42 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "libftprintf.h"

void		sys_siglog(int sig)
{
	char	*sigmess;

	sigmess = NULL;
	if (sig == SIGSEGV)
		sigmess = "Segmentation fault";
	else if (sig == SIGTERM || sig == SIGINT)
		sigmess = "Terminated";
	else if (sig == SIGKILL)
		sigmess = "Killed";
	else if (sig == SIGABRT)
		sigmess = "Abort trap";
	else if (sig == SIGBUS)
		sigmess = "Bus error";
	else
		sigmess = "Signaled";
	sigmess ? ft_printf("%s: %d\n", sigmess, sig) : 0;
}
