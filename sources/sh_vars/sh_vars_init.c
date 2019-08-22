/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_vars_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 13:01:11 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/22 18:51:15 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_vars.h"

void	free_tree_content(void *content)
{
	free(content);
}

void	sh_init_vars(int argc, char **argv, char **envp, t_envp *env)
{
	char	**spl;
	t_avln	*avln;

	sh_parser_argv(argc, argv, env);
	env->global = ft_avl_tree_create(&free_tree_content);
	env->local = ft_avl_tree_create(&free_tree_content);
	while (*envp)
	{
		spl = ft_strsplit(*envp, '=');
		ft_avl_set(env->global, ft_avl_node(spl[0], (char *)spl[1],\
					1 + ft_strlen(spl[1])));
		free(spl[0]);
		free(spl[1]);
		free(spl); //! MALLOC ERROR NEED TO FREE
		envp++;
	}
}
