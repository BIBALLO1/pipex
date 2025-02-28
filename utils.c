/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoraled <dmoraled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:57:02 by dmoraled          #+#    #+#             */
/*   Updated: 2025/02/28 09:39:16 by dmoraled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_path(char *env[])
{
	while (*env)
	{
		if (ft_strncmp(*env, "PATH", 4) == 0)
			return (ft_split(&(*env)[5], ':'));
		++env;
	}
	return (0);
}

void	free_tab(char **tab)
{
	char	**p;

	if (!tab)
		return ;
	p = tab;
	while (*p)
	{
		free(*p);
		p++;
	}
	free(tab);
}

char	*find_exec_path(char **path, char *name)
{
	char	*full_path;

	while (*path)
	{
		full_path = ft_strsjoin(*path, name, '/');
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		++path;
	}
	if (access(name, X_OK) == 0)
		return (ft_strdup(name));
	return (0);
}
