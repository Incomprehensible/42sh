/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_parse_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/24 06:35:36 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

void	*ft_arrmemdel(void **ap)
{
	unsigned char	**ptr;
	int				i;
	int				j;

	i = 0;
	j = 0;
	ptr = (unsigned char **)ap;
	while (ptr[j])
	{
		free(ptr[j]);
		j++;
	}
	free(ap);
	return (NULL);
}

char    *pull_token(char *str, size_t i)
{
    char *new;
    size_t tmp;

    tmp = i;
    new = (char *)ft_memalloc((i + 1));
    while (*str && i--)
        *new++ = *str++;
    return (new - tmp);
}

// cut by '\' - kek\\nkek
// we get kek and \nkek
//just skip \n in the front 

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

//В двойных кавычках он экранирует только " и $

static short	mirrored(char **arr, size_t i)
{
	// if (!ft_strchr(arr[i - 1], '"') && !ft_strchr(arr[i - 1], '$'))
	// 	if (!ft_strchr(arr[i], '"') && !ft_strchr(arr[i], '$'))
	if (!ft_strchr(arr[i], '"') && !ft_strchr(arr[i], '$'))
		return (0);
	return (1);
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
	while (splitted[i])
	{
		if (!mirrored(splitted, i))
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

// //в одинарных экранирует только одинарную кавычку

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
	if (!ft_strchr(str, '\\'))
		return (str);
	if (type == DQUOTS)
		str = cut_mirr_dq(str);
	else if (type == APOFS)
		str = cut_mirr_q(str);
	else
		str = cut_mirr(str);
	return (str);
}