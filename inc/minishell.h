/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:50 by aperez-b          #+#    #+#             */
/*   Updated: 2021/12/21 16:40:16 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/inc/libft.h"
# include "get_next_line.h"
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>

# define READ_END 0
# define WRITE_END 1

typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
	pid_t	pid;
	int		e_status;
}			t_prompt;

typedef struct s_mini
{
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}			t_mini;

enum	e_mini_error
{
	QUOTE = 1,
	NDIR = 2,
	NPERM = 3,
	NCMD = 6,
	DUPERR = 7,
	FORKERR = 8,
	PIPERR = 9,
	PIPENDERR = 10,
	MEM = 11
};

char	*mini_readline(t_prompt *prompt, char *str);

void	*mini_here_fd(t_prompt *prompt, int fd[2], int auxfd[2]);

int		builtin(t_prompt *prompt, t_list *cmd, int *is_exit, int n);

int		is_builtin(t_mini *n);

int		mini_cd(t_prompt *prompt);

int		mini_pwd(void);

int		mini_echo(t_list *cmd);

int		mini_export(t_prompt *prompt);

int		mini_unset(t_prompt *prompt);

int		mini_exit(t_list *cmd, int *is_exit);

void	*check_args(char *out, t_prompt *p);

char	**ft_cmdtrim(char const *s, char *set);

char	**ft_cmdsubsplit(char const *s, char *set);

char	*ft_strtrim_all(char const *s1, int squote, int dquote);

t_list	*fill_nodes(t_prompt *prompt, char **args, int i);

int		get_fd(t_prompt *prompt, int oldfd, char *path, int flags[2]);

t_mini	*get_outfile1(t_prompt *prompt, t_mini *node, char **args, int *i);

t_mini	*get_outfile2(t_prompt *prompt, t_mini *node, char **args, int *i);

t_mini	*get_infile1(t_prompt *prompt, t_mini *node, char **args, int *i);

t_mini	*get_infile2(t_prompt *prompt, t_mini *node, char **args, int *i);

void	*exec_cmd(t_prompt *prompt, t_list *cmd);

void	child_builtin(t_prompt *prompt, t_mini *n, int l, t_list *cmd);

void	exec_custom(char ***out, char *full, char *args, char **envp);

int		exec_builtin(t_prompt *prompt, int (*func)(t_prompt *));

void	get_cmd(t_prompt *prompt, t_list *start, char **split_path, char *path);

char	*expand_vars(char *str, int i, int quotes[2], t_prompt *prompt);

char	*expand_path(char *str, int i, int quotes[2], char *var);

int		get_here_doc(t_prompt *prompt, char *str[2], size_t len, char *aux[2]);

void	*mini_perror(t_prompt *prompt, int err, char *param);

char	*mini_getenv(char	*var, char **envp, int n);

char	**mini_setenv(char *var, char *value, char **envp, int n);

char	*mini_getprompt(t_prompt prompt);

void	free_content(void *content);

void	handle_sigint(int sig);

void	handle_sigint_child(int sig);

void	handle_sigquit(int sig);

#endif
