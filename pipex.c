/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoraled <dmoraled@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:17:22 by dmoraled          #+#    #+#             */
/*   Updated: 2025/02/28 14:07:08 by dmoraled         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

char	*get_valid_cmd(char *cmd[], char *path[])
{
	char	*pname;

	pname = find_exec_path(path, cmd[0]);
	if (!pname)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		free_tab(cmd);
		free_tab(path);
		exit(127);
	}
	return (pname);
}

pid_t	run_program(char *cmd[], char *path[], char *env[], int io[3])
{
	pid_t	pid;
	char	*pname;

	if (!cmd || !(*cmd))
	{
		free_tab(cmd);
		return (0);
	}
	pid = fork();
	if (pid != 0)
	{
		free_tab(cmd);
		return (pid);
	}
	dup2(io[0], 0);
	dup2(io[1], 1);
	close(io[2]);
	pname = get_valid_cmd(cmd, path);
	// pname = find_exec_path(path, cmd[0]);
	// if (!pname)
	// {
	// 	ft_putstr_fd("command not found: ", 2);
	// 	ft_putendl_fd(cmd[0], 2);
	// 	free_tab(cmd);
	// 	free_tab(path);
	// 	exit(127);
	// }
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
	if (io[0] < 0)
		perror(argv[1]);
	if (pipe(pipefd) != 0)
	{
		perror("pipe");
		exit(1);
	}
}

int	get_retval(int last_ret, int io[2])
{
	int	v;

	if (WIFEXITED(last_ret))
		v = WEXITSTATUS(last_ret);
	if (io[1] < 0)
		v = 1;
	return (v);
}

int	main(int argc, char *argv[], char *env[])
{
	int		io[2];
	int		pipefd[2];
	pid_t	pids[2];
	char	**path;
	int		ret_val;

	init(argc, argv, io, pipefd);
	path = get_path(env);
	pids[0] = run_program(ft_split(argv[2], ' '), path, env,
			(int [3]){io[0], pipefd[1], pipefd[0]});
	io[1] = open(argv[4], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (io[1] < 0)
		perror(argv[4]);
	pids[1] = run_program(ft_split(argv[3], ' '), path, env,
			(int [3]){pipefd[0], io[1], pipefd[1]});
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pids[0], 0, 0);
	waitpid(pids[1], &ret_val, 0);
	free_tab(path);
	return (get_retval(ret_val, io));
}
