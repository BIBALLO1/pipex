/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoraled <dmoraled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:17:22 by dmoraled          #+#    #+#             */
/*   Updated: 2025/02/03 18:01:44 by dmoraled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void	arglog(char *arg[])
{
	while (*arg)
	{
		ft_putendl_fd(*arg, 2);
		++arg;
	}
}

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

char	*find_path(char **path, char *name)
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

pid_t	run_program(char *cmd[], char *path[], char *env[], int inout[2])
{
	pid_t	pid;
	char	*pname;

	pid = fork();
	if (pid == 0)
	{
		dup2(inout[1], 1);
		dup2(inout[0], 0);
		pname = find_path(path, cmd[0]);
		free(cmd[0]);
		cmd[0] = pname;
		arglog(cmd);
		execve(cmd[0], cmd, env);
	}
	return (pid);
}

int	main(int argc, char *argv[], char *env[])
{
	int		outfd;
	int		infd;
	int		pipefd[2];
	pid_t	pids[2];
	char	**path;

	if (argc != 5)
		return (1);
	infd = open(argv[1], O_RDONLY);
	outfd = open(argv[4], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (pipe(pipefd) != 0)
		ft_putendl_fd("pipe error : (", 2);
	path = get_path(env);
	pids[0] = run_program(ft_split(argv[2], ' '), path, env, (int []){infd, pipefd[1]});
	pids[1] = run_program(ft_split(argv[3], ' '), path, env, (int []){pipefd[0], outfd});
	ft_putstr_fd("waiting", 2);
	waitpid(pids[0], 0, 0);
	waitpid(pids[1], 0, 0);
	ft_putendl_fd(".", 2);
	return (0);
}
