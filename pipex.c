/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoraled <dmoraled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:17:22 by dmoraled          #+#    #+#             */
/*   Updated: 2025/02/12 13:57:46 by dmoraled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void	arglog(char *arg[])
{
	while (*arg)
	{
		ft_putstr_fd(*arg, 2);
		ft_putstr_fd(" ", 2);
		++arg;
	}
	ft_putendl_fd("", 2);
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

pid_t	run_program(char *cmd[], char *path[], char *env[], int io[3])
{
	pid_t	pid;
	char	*pname;

	pid = fork();
	if (pid != 0)
		return (pid);
	dup2(io[0], 0);
	dup2(io[1], 1);
	close(io[2]);
	pname = find_path(path, cmd[0]);
	free(cmd[0]);
	cmd[0] = pname;
	arglog(cmd);
	execve(cmd[0], cmd, env);
	return (pid);
}

int	main(int argc, char *argv[], char *env[])
{
	int		io[2];
	int		pipefd[2];
	pid_t	pids[2];
	char	**path;

	if (argc != 5)
	{
		ft_putendl_fd("Invalid argument count", 2);
		exit(1);
	}
	io[0] = open(argv[1], O_RDONLY);
	if (io[0] < 0)
	{
		perror("input file");
		exit(1);
	}
	io[1] = open(argv[4], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (io[1] < 0)
	{
		perror("output file");
		exit(1);
	}
	if (pipe(pipefd) != 0)
	{
		perror("pipe");
		exit(1);
	}
	path = get_path(env);
	pids[0] = run_program(ft_split(argv[2], ' '), path, env,
			(int []){io[0], pipefd[1], pipefd[0]});
	pids[1] = run_program(ft_split(argv[3], ' '), path, env,
			(int []){pipefd[0], io[1], pipefd[1]});
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pids[0], 0, 0);
	waitpid(pids[1], 0, 0);
	return (0);
}
