/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_constructors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/23 17:09:41 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static char	*pull_first(char *str, char *second, char *third)
{
	char	*tmp;
	char	*new;

	if (*str == '\\')
	{
		new = ft_strjoin("\\", second);
		tmp = ft_strjoin(new, "\\");
		free(new);
		new = ft_strjoin(tmp, third);
	}
	else
	{
		tmp = ft_strjoin(second, "\\");
		new = ft_strjoin(tmp, third);
	}
	free(tmp);
	return (new);
}

static char	*choose_offset(size_t i, char **splitted)
{
	char	*tmp;

	if (splitted[i] && splitted[i][0] == '\n')
		tmp = &splitted[i][1];
	else
		tmp = splitted[i];
	return (tmp);
}

static char	*cut_mirr_dq(char *str, char **splitted)
{
	char	*new;
	char	*tmp;
	size_t	i;

	i = 1;
	new = NULL;
	if (!splitted[i] && splitted[0])
		new = pull_single(splitted[0]);
	while (splitted[i])
	{
		if (!mirrored(splitted[i]))
		{
			if (new)
				new = cook_mirr(new, splitted[i]);
			else
				new = pull_first(str, splitted[i - 1], splitted[i]);
		}
		else
		{
			tmp = choose_offset(i, splitted);
			new = join_mirr(new, tmp, splitted[i - 1]);
		}
		i++;
	}
	return (new);
}

static char	*cut_mirr_q(char **splitted)
{
	char	*new;
	char	*tmp;
	size_t	i;

	i = 1;
	new = NULL;
	if (!splitted[i] && splitted[0])
		new = pull_single(splitted[0]);
	while (splitted[i])
	{
		if (!ft_strchr(splitted[i], '\''))
		{
			if (new)
				new = cook_mirr(new, splitted[i]);
			else
			{
				tmp = ft_strjoin(splitted[i - 1], "\\");
				new = join_mirr(tmp, splitted[i], NULL);
			}
		}
		else
			new = join_mirr(new, splitted[i], splitted[i - 1]);
		i++;
	}
	return (new);
}

char		*cutting_mirr_station(char *str, t_tk_type type)
{
	char	*tmp;
	char	**splitted;
	char	*new;

	if (!ft_strchr(str, '\\'))
		return (str);
	splitted = ft_strsplit(str, '\\');
	if (!splitted || !splitted[0])
	{
		if (splitted)
			ft_arrmemdel((void **)splitted);
		return (NULL);
	}
	if (type == DQUOTS)
		new = cut_mirr_dq(str, splitted);
	else if (type == APOF)
		new = cut_mirr_q(splitted);
	else
		new = cut_mirr(splitted);
	ft_arrmemdel((void **)splitted);
	free(str);
	return (new);
}
