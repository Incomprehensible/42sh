/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_mirror_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 21:45:58 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char			*add_slashes(char *strt, short flag, t_tk_type type)
{
	char	*tmp;

	if (flag)
	{
		if ((flag == 1 || flag == 3) && type != TK_EXPR)
		{
			tmp = ft_strjoin("\\", strt);
			free(strt);
			strt = tmp;
		}
		if (flag == 2 || flag == 3)
		{
			tmp = ft_strjoin(strt, "\\");
			free(strt);
			strt = tmp;
		}
	}
	return (strt);
}

static short	get_flag(char *strt, char *fin, size_t i)
{
	short	flag;

	flag = 0;
	if (*fin && *fin == '\\' && *(fin + 1) == '\\')
		flag = 2;
	if (*(strt - i) == '\\' && !(mirrored(strt - i + 1)))
		flag = flag ? 3 : 1;
	return (flag);
}

char			*get_copy(char *strt, char *fin, t_tk_type type)
{
	short	flag;
	char	*str;
	char	*tmp;
	size_t	i;

	i = 0;
	if (strt == fin)
		return (ft_strdup("\\"));
	while (strt != fin)
	{
		strt++;
		i++;
	}
	flag = get_flag(strt, fin, i);
	strt -= i;
	str = ft_strnew(i + 1);
	ft_strlcat(str, strt, i + 1);
	if (!ft_strchr(str, '\\'))
		return (add_slashes(str, flag, type));
	tmp = cutting_mirr_station(str, type);
	str = add_slashes(tmp, flag, type);
	return (str);
}

static void		pull_copy(char **tmp, char *start, char *fin, t_tk_type tp)
{
	char	*tmp2;
	char	*new;

	if (!tmp || !(*tmp))
		*tmp = get_copy(start, fin, tp);
	else
	{
		tmp2 = get_copy(start, fin, tp);
		new = ft_strjoin(*tmp, tmp2);
		free(*tmp);
		free(tmp2);
		*tmp = new;
	}
}

char			*markup_station(char *str, t_tk_type type)
{
	char	*start;
	char	*fin;
	char	*tmp;

	fin = NULL;
	tmp = NULL;
	if (!ft_strchr(str, '\\'))
		return (ft_strdup(str));
	while (*str)
	{
		start = str;
		while (!fin && *str)
		{
			if (*str == '\\' && *(++str) == '\\')
				fin = str - 1;
			str++;
		}
		fin = fin ? fin : str;
		pull_copy(&tmp, start, fin, type);
		fin = NULL;
	}
	return (tmp);
}
