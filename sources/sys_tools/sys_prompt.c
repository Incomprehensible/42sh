/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 08:08:18 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 08:10:14 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_errors.h"
#include "dstring.h"
#include "parser.h"
#include <time.h>
#include "env.h"

void	put_prompt_bolder(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	int		len;
	char	*ptr;
	char	buff[6];

	len = 2;
	ptr = (*pr_src)->txt + ind + 1;
	if (*ptr == 'b')
		ft_strcpy(buff, "\033[2m");
	else if (*ptr == 'B')
		ft_strcpy(buff, "\033[1m");
	else
		ft_strcpy(buff, "\033[22m");
	dstr_slice_del(pr_src, ind, ind + len);
	*ofs = (size_t)dstr_insert_str(*pr_src, (char *)buff, ind);
}

void	put_prompt_sys_path_uh(DSTRING **pr_src, size_t ind, size_t *ofs)
{
	char			info[PROMPT_MAX_INFO_BUFF];
	char			*ptr;
	int				err;

	err = 0;
	ptr = (*pr_src)->txt + ind + 1;
	if ((*ptr == 'h' || *ptr == 'H') && (ptr = info))
		err = gethostname(info, PROMPT_MAX_INFO_BUFF);
	if (*ptr == 'u' && (ptr = info))
		err = getlogin_r(info, PROMPT_MAX_INFO_BUFF);
	if ((*ptr == 'p' || *ptr == 'P') && (ptr = info))
		err = getcwd(info, PROMPT_MAX_INFO_BUFF) ? 0 : 1;
	if ((*pr_src)->txt[ind + 1] == 'H' && !err)
	{
		if ((ptr = ft_strchr(info, '.')))
			(*ptr) = '\0';
		ptr = info;
	}
	if ((*pr_src)->txt[ind + 1] == 'P' && !err)
		ptr = ft_basename(info);
	dstr_slice_del(pr_src, ind, ind + 2);
	!err ? *ofs = (size_t)dstr_insert_str(*pr_src, ptr, ind) : 0;
	ptr != info ? free(ptr) : 0;
}

char	*prompt_get_status(ENV *env)
{
	DSTRING	*buff;
	char	*ret;

	buff = env_get_variable("?", env);
	if (buff->strlen == 0 || (0 == ft_atoll_base(buff->txt, 10)))
		ret = ft_strdup(PROMPT_OK_STATUS_C);
	else
		ret = ft_strdup(PROMPT_KO_STATUS_C);
	dstr_del(&buff);
	return (ret);
}

char	*prompt_get_time(void)
{
	time_t		timer;
	char		*str;
	char		*buff;

	timer = time(NULL);
	str = ft_strdup("HH:MM");
	if (!str)
		return (0);
	buff = ctime(&timer);
	str[0] = buff[11];
	str[1] = buff[12];
	str[3] = buff[14];
	str[4] = buff[15];
	return (str);
}

void	put_prompt_time_status(DSTRING **pr, size_t ind, size_t *ofs, ENV *env)
{
	char			*info;
	char			*ptr;

	ptr = (*pr)->txt + ind + 1;
	if (*ptr == 't')
		info = prompt_get_time();
	else
		info = prompt_get_status(env);
	dstr_slice_del(pr, ind, ind + 2);
	*ofs = (size_t)dstr_insert_str(*pr, info, ind);
	free(info);
}
