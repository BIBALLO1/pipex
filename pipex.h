/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoraled <dmoraled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:57:13 by dmoraled          #+#    #+#             */
/*   Updated: 2025/02/18 12:00:50 by dmoraled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"

# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>

char	**get_path(char *env[]);
char	*find_exec_path(char **path, char *name);
void	free_tab(char **tab);

#endif
