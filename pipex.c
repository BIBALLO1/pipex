/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoraled <dmoraled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:17:22 by dmoraled          #+#    #+#             */
/*   Updated: 2025/02/18 12:40:26 by dmoraled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

pid_t	run_program(char *cmd[], char *path[], char *env[], int io[3])
{
	pid_t	pid;
	char	*pname;

	pid = fork();
	if (pid != 0)
	{
		free_tab(cmd);
		return (pid);
	}
	dup2(io[0], 0);
	dup2(io[1], 1);
	close(io[2]);
	pname = find_exec_path(path, cmd[0]);
	if (!pname)
	{
		perror(cmd[0]);
		free_tab(cmd);
		free_tab(path);
		exit(1);
	}
	free(cmd[0]);
	cmd[0] = pname;
	execve(cmd[0], cmd, env);
	return (pid);
}

void	init(int argc, char *argv[], int io[2], int pipefd[2])
{
	if (argc != 5)
	{
		ft_putendl_fd("Usage: <infile> <cmd1> <cmd2> <outfile>", 2);
		exit(1);
	}
	io[0] = open(argv[1], O_RDONLY);
	io[1] = open(argv[4], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (io[0] < 0)
		perror("input file");
	if (io[1] < 0)
		perror("output file");
	if (io[0] < 0 || io[1] < 0)
		exit(1);
	if (pipe(pipefd) != 0)
	{
		perror("pipe");
		exit(1);
	}
}

int	main(int argc, char *argv[], char *env[])
{
	int		io[2];
	int		pipefd[2];
	pid_t	pids[2];
	char	**path;

	init(argc, argv, io, pipefd);
	path = get_path(env);
	pids[0] = run_program(ft_split(argv[2], ' '), path, env,
			(int [3]){io[0], pipefd[1], pipefd[0]});
	pids[1] = run_program(ft_split(argv[3], ' '), path, env,
			(int [3]){pipefd[0], io[1], pipefd[1]});
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pids[0], 0, 0);
	waitpid(pids[1], 0, 0);
	free_tab(path);
	return (0);
}
