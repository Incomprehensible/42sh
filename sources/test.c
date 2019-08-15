char			ft_isdir(const char *path)
{
	DIR			*dir;
	char		*dir_name;

	dir_name = ft_concat(2, "00", "./", path);
	dir = opendir(dir_name);
	if (!dir)
	{
		free(dir_name);
		return (0);
	}
	free(dir_name);
	closedir(dir);
	return (1);
}
// 1. запомнить позицию курсора
// 2. напечатать таблицу
// 3. вернуть курсор на мето
// 4. при повторном нажатии дописать путь или комнду