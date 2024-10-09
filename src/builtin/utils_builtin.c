/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/08 17:53:35 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_err_state(char *tmp)
{
	char	*err;
	char	*tmp2;

	err = ft_itoa(g_err_state);
	tmp2 = ft_strtrim2(tmp, "=");
	tmp = ft_strtrim2(tmp2, "?");
	tmp2 = ft_strndup(tmp, ft_strlen(tmp));
	free(tmp);
	tmp = ft_strjoin(err, tmp2);
	free(tmp2);
	free(err);
	return (tmp);
}

static	int	ft_isalpha_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if ((str[i] <= 122 && str[i] >= 97) || (str[i] <= 90 && str[i] >= 65))
			i++;
		else if (str[i] == '_')
			i++;
		else if (str[i] != '\0')
			return (i);
	}
	return (-1);
}

char	*tmp_set(char *val)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	tmp = ft_strndup(val, ft_strlen(val));
	tmp2 = ft_strtrim2(tmp, "$");
	if (*tmp2 && *tmp2 == '?')
		return (tmp2);
	i = ft_isalpha_len(tmp2);
	// if (i > 0)
	// 	tmp = ft_strndup(tmp2, i);
	// else
	// 	tmp = ft_strndup(tmp2, ft_strlen(tmp2));
	if (i > 0 && tmp2[i] && tmp2[i] != '=')
	{
		tmp = ft_strtrim2(tmp2, &tmp2[i]);
		tmp2 = ft_strjoin(tmp, "=");
		return (ft_free_null(tmp), tmp2);
	}
	else if (i > 0 && tmp2[i] && tmp2[i] == '=')
		tmp = ft_strndup(tmp2, i + 1);
	ft_free_null(tmp2);
	return (tmp);
}

void	process_command2(t_data **data, char **command)
{
	if ((*data)->merdoso == 0)
		(*data)->cmd2 = find_cmd(command[0], data);
	else if ((*data)->merdoso == 1)
		(*data)->merdoso = 0;
}

static	int	redirect_builtin(t_data **data)
{
	if (!((*data)->fd < 0))
	{
		if ((*data)->redirect_state == 1)
		{
			(*data)->saved_fd = dup(STDOUT_FILENO);
			if (dup2((*data)->fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		if ((*data)->redirect_state == 0)
		{
			(*data)->saved_fd = dup(STDIN_FILENO);
			if (dup2((*data)->fd, STDIN_FILENO) < 0)
				return (-1);
		}
		//close((*data)->fd);
		//close(STDOUT_FILENO);
	}
	return (0);
}

int	manual_cmd(char **cmd_args, t_data **data, t_token **token)
{
	t_data	*tmp;

	tmp = (*data);
	if (redirect_builtin(data) < 0)
		return (-1);
	///////////////////
	close((*data)->fd);
	///////////////////
	tmp->cmd = conf_man_cmd(cmd_args[0]);
	(*data)->cmd_args = NULL;
	clean_qt(token);
	if (tmp->cmd == CH_DIR)
		return (ft_remove_ws(token), cd_cmd(data, token), 1);
	if (tmp->cmd == ECHO)
		return (echo_cmd(token), 1);
	if (tmp->cmd == EXPORT)
		return (export_cmd(data, token), 1);
	if (tmp->cmd == UNSET)
		return (unset_env(token, &tmp->env_list), 1);
	if (tmp->cmd == ENV && !(*data)->merdoso)
		return (env_cmd(data), 1);
	if (tmp->cmd == EXIT)
		return (cmd_exit(data, token), 1);
	if (tmp->cmd == PWD)
		return (pwd_cmd(), 1);
	return (0);
}

int	conf_man_cmd(char *str)
{
	if (ft_strncmp(str, "cd", ft_strlen(str)) == 0)
		return (1);
	if (ft_strncmp(str, "echo", ft_strlen(str)) == 0)
		return (2);
	if (ft_strncmp(str, "export", ft_strlen(str)) == 0)
		return (3);
	if (ft_strncmp(str, "unset", ft_strlen(str)) == 0)
		return (4);
	if (ft_strncmp(str, "env", ft_strlen(str)) == 0)
		return (5);
	if (ft_strncmp(str, "exit", ft_strlen(str)) == 0)
		return (6);
	if (ft_strncmp(str, "pwd", ft_strlen(str)) == 0)
		return (7);
	else
		return (0);
}
