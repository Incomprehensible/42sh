/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_options_fsm.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 17:08:01 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/25 17:17:24 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_options.h"
#include "rms.h"
#include "bltn.h"

void			et_rm_clear_opt(t_opt *opt)
{
	ft_dlst_delf(&(opt->ops), 0, et_rm_str);
	ft_dlst_delf(&(opt->params), 0, et_rm_str);
	ft_dlst_delf(&(opt->lib_fs), 0, et_rm_str);
	ft_bzero(opt, sizeof(t_opt));
}

t_state			stt_hnd_err(int *ac, char **av, t_opt *opt)
{
	t_state		stt;

	et_rm_clear_opt(opt);
	av = NULL;
	ac = NULL;
	stt = stt_null;
	return (stt);
}

int				opt_init(int ac, char **av, t_opt *opt)
{
	int				i;
	t_state			stt;
	t_stt_handler	arr_hndls[OPT_FSM_STT_LIM];

	i = 1;
	stt = 1;
	arr_hndls[stt_fin] = stt_hnd_fin;
	arr_hndls[stt_select] = stt_hnd_select;
	arr_hndls[stt_ops] = stt_hnd_ops;
	arr_hndls[stt_lb] = stt_hnd_lib;
	arr_hndls[stt_fl] = stt_hnd_prm;
	arr_hndls[stt_prm] = stt_hnd_prm;
	arr_hndls[stt_err] = stt_hnd_err;
	ft_bzero(opt, sizeof(t_opt));
	while (stt != stt_null)
	{
		if (i >= ac)
			return (-1);
		stt = arr_hndls[stt](&i, av, opt);
	}
	return (0);
}
