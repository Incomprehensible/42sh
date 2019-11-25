/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_options_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 17:07:59 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/25 17:43:39 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_options.h"
#include "ft_string.h"

t_state			stt_hnd_fin(int *ac, char **av, t_opt *opt)
{
	t_state		stt;

	stt = stt_select;
	opt = NULL;
	if (av[*ac] == NULL)
		stt = stt_null;
	return (stt);
}

t_state			stt_hnd_select(int *ac, char **av, t_opt *opt)
{
	t_state		stt;

	opt = NULL;
	if (opt_is_null(av[*ac]))
		stt = stt_fin;
	else if (opt_is_opsfl(av[*ac]) && ++(*ac))
		stt = stt_ops;
	else if (opt_is_libfl(av[*ac]) && ++(*ac))
		stt = stt_lb;
	else
		stt = stt_fl;
	return (stt);
}

t_state			stt_hnd_ops(int *ac, char **av, t_opt *opt)
{
	t_state		stt;
	t_dlist		*new_item;

	if (opt_is_flend(av[*ac]))
		stt = stt_select;
	else if (opt_is_null(av[*ac]))
		stt = stt_fin;
	else
	{
		stt = stt_ops;
		new_item = ft_dlstnew(av[*ac], ft_strlen(av[*ac]) + 1);
		if (!new_item)
			stt = stt_err;
		else
			ft_dlstpush(&(opt->ops), new_item);
	}
	*ac += 1;
	return (stt);
}

t_state			stt_hnd_lib(int *ac, char **av, t_opt *opt)
{
	t_state		stt;
	t_dlist		*new_item;

	if (opt_is_flend(av[*ac]))
		stt = stt_select;
	else if (opt_is_null(av[*ac]))
		stt = stt_fin;
	else
	{
		stt = stt_lb;
		new_item = ft_dlstnew(av[*ac], ft_strlen(av[*ac]) + 1);
		if (!new_item)
			stt = stt_err;
		else
			ft_dlstpush(&(opt->lib_fs), new_item);
	}
	*ac += 1;
	return (stt);
}

t_state			stt_hnd_prm(int *ac, char **av, t_opt *opt)
{
	t_state		stt;
	t_dlist		*new_item;

	stt = stt_prm;
	if (opt_is_null(av[*ac]))
		return (stt_fin);
	new_item = ft_dlstnew(av[*ac], ft_strlen(av[*ac]) + 1);
	if (!new_item)
		stt = stt_err;
	else
		ft_dlstpush(&(opt->params), new_item);
	*ac += 1;
	return (stt);
}
