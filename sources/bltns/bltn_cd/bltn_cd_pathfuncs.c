#include "bltn.h"

void		et_rm_str(void *s, size_t l)
{
	l = 0;
	if (s)
		free(s);
}

DSTRING		*bltn_cd_pathtostr(t_dlist *path)
{
	DSTRING	*res;
	char	*val;

	val = (char *)path->content;
	if (val[0] != '/')
		res = dstr_new("/");
	else
	{
		res = dstr_new(val);
		path = path->next;
	}
	while (path)
	{
		if (ft_strlen((char *)path->content) != 0)
			dstr_insert_str(res, (char *)path->content, res->strlen);
		if (path->next != NULL)
			dstr_insert_ch(res, '/', res->strlen);
		path = path->next;
	}
	return (res);
}

void		bltn_cd_destroy_path(t_dlist **path)
{
	if ((*path))
	{
		ft_dlst_delf(&(*path), 0, et_rm_str);
		(*path) = NULL;
	}
}