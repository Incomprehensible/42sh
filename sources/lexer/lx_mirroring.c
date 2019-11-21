/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_mirroring.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/21 00:01:56 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static char	*cut_mirr(char *str)
{
	char	**splitted;
	char	*new;
	char	*tmp;
	size_t	i;

	i = 1;
	new = NULL;
	splitted = ft_strsplit(str, '\\');
	if (!splitted || !splitted[0])
		return (NULL);
	else if (!splitted[i] && splitted[0])
		new = pull_single(splitted[0]);
	while (splitted[i])
	{
		if (splitted[i] && splitted[i][0] == '\n')
			tmp = &splitted[i][1];
		else
			tmp = splitted[i];
		if (new)
		{
			tmp = ft_strjoin(new, tmp);
			free(new);
			new = tmp;
		}
		else
			new = ft_strjoin(splitted[i - 1], tmp);
		i++;
	}
	free(str);
	ft_arrmemdel((void **)splitted);
	return (new);
}

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

static char	*cut_mirr_dq(char *str)
{
	char	**splitted;
	char	*new;
	char	*tmp;
	size_t	i;

	i = 1;
	new = NULL;
	splitted = ft_strsplit(str, '\\');
	if (!splitted || !splitted[0])
		return (NULL);
	else if (!splitted[i] && splitted[0])
		new = pull_single(splitted[0]);
	while (splitted[i])
	{
		if (!mirrored(splitted[i]))
		{
			if (new)
			{
				tmp = ft_strjoin(new, "\\");
				free(new);
				new = tmp;
				tmp = ft_strjoin(new, splitted[i]);
				free(new);
				new = tmp;
			}
			else
				new = pull_first(str, splitted[i - 1], splitted[i]);
		}
		else
		{
			if (splitted[i] && splitted[i][0] == '\n')
				tmp = &splitted[i][1];
			else
				tmp = splitted[i];
			if (new)
			{
				tmp = ft_strjoin(new, tmp);
				free(new);
				new = tmp;
			}
			else
				new = ft_strjoin(splitted[i - 1], tmp);
		}
		i++;
	}
	free(str);
	ft_arrmemdel((void **)splitted);
	return (new);
}

static char	*cut_mirr_q(char *str)
{
	char	**splitted;
	char	*new;
	char	*tmp;
	size_t	i;

	i = 1;
	new = NULL;
	splitted = ft_strsplit(str, '\\');
	if (!splitted || !splitted[0])
		return (NULL);
	else if (!splitted[i] && splitted[0])
		new = pull_single(splitted[0]);
	while (splitted[i])
	{
		if (!ft_strchr(splitted[i], '\''))
		{
			if (new)
			{
				tmp = ft_strjoin(new, "\\");
				free(new);
				new = tmp;
				tmp = ft_strjoin(new, splitted[i]);
				free(new);
				new = tmp;
			}
			else
			{
				tmp = ft_strjoin(splitted[i - 1], "\\");
				new = ft_strjoin(tmp, splitted[i]);
				free(tmp);
			}
		}
		else
		{
			if (new)
			{
				tmp = ft_strjoin(new, splitted[i]);
				free(new);
				new = tmp;
			}
			else
				new = ft_strjoin(splitted[i - 1], splitted[i]);
		}
		i++;
	}
	free(str);
	ft_arrmemdel((void **)splitted);
	return (new);
}

char	*cutting_mirr_station(char *str, t_tk_type type)
{
	char *tmp;

	if (!ft_strchr(str, '\\'))
	{
		tmp = ft_strdup(str);
		free(str);
		return (tmp);
	}
	if (type == DQUOTS)
		str = cut_mirr_dq(str);
	else if (type == APOF)
		str = cut_mirr_q(str);
	else
		str = cut_mirr(str);
	return (str);
}

char	*add_slashes(char *strt, short flag, t_tk_type type)
{
	char	*tmp;

	type = 0;
	if (flag)
	{
		if (flag == 1 || flag == 3)
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

char	*get_copy(char *strt, char *fin, t_tk_type type)
{
	short 	flag;
	char	*str;
	char	*tmp;
	size_t	i;

	flag = 0;
	i = 0;
	if (strt == fin)
		return (ft_strdup("\\"));
	while (strt != fin)
	{
		strt++;
		i++;
	}
	if (*fin && *fin == '\\' && *(fin + 1) == '\\')
		flag = 2;
	if (*(strt - i) == '\\' && !(mirrored(strt - i + 1)))
		flag = flag ? 3 : 1;
	strt -= i;
	str = ft_strnew(i + 1);
	ft_strlcat(str, strt, i + 1);
	if (!ft_strchr(str, '\\'))
		return (add_slashes(str, flag, type));
	tmp = cutting_mirr_station(str, type);
	str = add_slashes(tmp, flag, type);
	return (str);
}

char	*markup_station(char *str, t_tk_type type)
{
	char	*start;
	char	*fin;
	char	*tmp;
	char	*tmp2;
	char	*new;

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
		if (!tmp)
			tmp = get_copy(start, fin, type);
		else
		{
			tmp2 = get_copy(start, fin, type);
			new = ft_strjoin(tmp, tmp2);
			free(tmp);
			free(tmp2);
			tmp = new;
		}
		fin = NULL;
	}
	return (tmp);
}