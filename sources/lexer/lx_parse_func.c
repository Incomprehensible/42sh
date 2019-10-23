/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_parse_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/23 09:35:13 by bomanyte         ###   ########.fr       */
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
	// if (splitted[1] && splitted[1][0] == '\n')
	// 	tmp = &splitted[1][0];
	// else
	// 	tmp = splitted[1];
	//new = ft_strjoin(splitted[0], tmp);
	free(str);
	ft_arrmemdel((void **)splitted);
	return (new);
}

//в двойных кавычках "\n" это так и есть
//В двойных кавычках он экранирует только двойные кавычки и $

// char	*cut_mirr_dq(char *str)
// {
	
// }

// //в одинарных экранирует только одинарную кавычку

// char	*cut_mirr_q(char *str)
// {
	
// }

char	*cutting_mirr_station(char *str, t_tk_type type)
{
	if (!ft_strchr(str, '\\'))
		return (str);
	// if (type == DQUOTS)
	// 	str = cut_mirr_dq(str);
	// else if (type == APOFS)
	// 	str = cut_mirr_q(str);
	// else
		str = cut_mirr(str);
	return (str);
}