/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:02:33 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/22 18:46:06 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_fds[2][2];

static char	*get_home(t_prompt prompt)
{
	char	*temp;
	char	*pwd;
	char	*home;

	pwd = getcwd(NULL, 0);
	home = mini_getenv("HOME", prompt.envp, 4);
	if (home && home[0] && ft_strnstr(pwd, home, ft_strlen(pwd))) //домашняя директория равна ~
	{
		temp = pwd;
		pwd = ft_strjoin("~", &pwd[ft_strlen(home)]); // к домашней директории приюавляем pwd
		free(temp);
	}
	free(home);
	home = ft_strjoin("-> ", pwd);
	free(pwd);
	pwd = ft_strjoin(home, " ");
	free(home);
	home = ft_strjoin(" ", pwd);
	free(pwd);
	pwd = ft_strjoin(home, "");
	free(home);
	return (pwd);
}

static char	*get_user(t_prompt prompt)
{
	char	**user;
	char	*tmp;
	//char	*temp2;

	user = NULL;
	//temp2 = NULL;
	exec_custom(&user, "/usr/bin/whoami", "whoami", prompt.envp);
	if (!user)
		user = ft_extend_matrix(user, "guest");
	// if (!ft_strncmp(user[0], "root", 4))
	// 	temp2 = ft_strjoin(NULL, RED);
	// else if ((int)(user[0][0]) % 5 == 0)
	// 	temp2 = ft_strjoin(NULL, CYAN);
	// else if ((int)(user[0][0]) % 5 == 1)
	// 	temp2 = ft_strjoin(NULL, GRAY);
	// else if ((int)(user[0][0]) % 5 == 2)
	// 	temp2 = ft_strjoin(NULL, GREEN);
	// else if ((int)(user[0][0]) % 5 == 3)
	// 	temp2 = ft_strjoin(NULL, MAGENTA);
	// else
	// 	temp2 = ft_strjoin(NULL, YELLOW);
	tmp = ft_strjoin(tmp, *user);
	//free(temp2);
	ft_free_matrix(&user);
	return (tmp);
}

char	*mini_getprompt(t_prompt prompt)
{
	char	*tmp;
	char	*tmp2;
	char	*aux;

	tmp = get_user(prompt);
	tmp2 = ft_strjoin(tmp, "@minishell");
	free(tmp);
	aux = get_home(prompt);
	tmp = ft_strjoin(tmp2, aux);
	free(aux);
	free(tmp2);
	// if (!prompt.e_status || prompt.e_status == -1)
	// 	tmp2 = ft_strjoin(tmp, BLUE);
	// else
	// 	tmp2 = ft_strjoin(tmp, RED);
	// free(tmp);
	//tmp = ft_strjoin(tmp2, "$ ");
	tmp = ft_strjoin(tmp, "$ ");
	// free(tmp2);
	// tmp2 = ft_strjoin(tmp, DEFAULT);
	// free(tmp);
	//return (tmp2);
	return (tmp);
}

void	readline_child(t_prompt *prompt, char *str, char *out, int is_null)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	close(g_fds[0][READ_END]);
	close(g_fds[1][READ_END]);
	out = readline(str);
	write(g_fds[0][WRITE_END], out, ft_strlen(out));
	is_null = !out;
	write(g_fds[1][WRITE_END], &prompt->e_status, sizeof(int)); // e_status - чтатус последней выполненной команды
	free(out);
	close(g_fds[0][WRITE_END]);
	close(g_fds[1][WRITE_END]);
	exit(is_null); // Если 0 - EXIT_SUCCESS, если 1 - EXIT_FILURE
}

char	*mini_readline(t_prompt *prompt, char *str)
{
	pid_t	pid;
	int		is_null;
	char	*out;

	is_null = 0;
	out = NULL;
	if (!mini_here_fd(prompt, g_fds[0], g_fds[1]))
		return (NULL);
	pid = fork();
	if (pid == -1)
		mini_perror(prompt, FORKERR, NULL);
	if (!pid)
		readline_child(prompt, str, out, is_null);
	close(g_fds[0][WRITE_END]);
	close(g_fds[1][WRITE_END]);
	waitpid(pid, &is_null, 0);
	out = get_next_line(g_fds[0][READ_END]);
	if (!is_null && !out)
		out = ft_strdup("");
	read(g_fds[1][READ_END], &prompt->e_status, sizeof(int));
	close(g_fds[0][READ_END]);
	close(g_fds[1][READ_END]);
	return (out);
}
