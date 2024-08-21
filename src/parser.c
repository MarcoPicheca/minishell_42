/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/21 19:57:14 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*find_cmd(char *cmd, t_data *data)
{
	int		i;
	char	*tmp;
	char	*holder;

	i = 0;
	while (data->my_paths[i])
	{
		tmp = ft_strjoin(data->my_paths[i], "/");
		holder = ft_strjoin(tmp, cmd);
		if (access(holder, X_OK) == 0)
			return (free(tmp), holder);
		free(tmp);
		free(holder);
		i++;
	}
	write(2, "command not found : ", 20);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	return (NULL);
}

int	conf_man_cmd(char *str)
{
	if (ft_strncmp(str, "cd", ft_strlen(str)) == 0)
		return (1);
	if (!ft_strncmp(str, "echo", ft_strlen(str)))
		return (2);
	if (!ft_strncmp(str, "export", ft_strlen(str)))
		return (3);
	if (!ft_strncmp(str, "unset", ft_strlen(str)))
		return (4);
	if (ft_strncmp(str, "env", ft_strlen(str)) == 0)
		return (5);
	if (!ft_strncmp(str, "exit", ft_strlen(str)))
		return (6);
	if (!ft_strncmp(str, "pwd", ft_strlen(str)))
		return (7);
	else
		return (0);
}

int	manual_cmd(char **cmd_args, t_data **data)
{
	int	i;
	t_data *tmp;

	tmp = *data;
	i = 0;
	tmp->cmd = conf_man_cmd(cmd_args[0]);
	if (tmp->cmd == CH_DIR)
		return (cd_cmd(cmd_args, data));
		// return (1);
	if (tmp->cmd == ECHO)
		return (1);
		// return (echo_cmd(cmd_args));
	if (tmp->cmd == EXPORT)
		return (1);
		// return (export_cmd(cmd_args));
	if (tmp->cmd == UNSET)
		return (1);
		// return (unset_cmd(cmd_args));
	if (tmp->cmd == ENV)
		return (env_cmd(data));
		// return (env_cmd(cmd_args));
	if (tmp->cmd == EXIT)
		return (1);
	if (tmp->cmd == PWD)
		return (pwd_cmd(data));
		// return (exit_cmd(cmd_args));
	return (0);
}

static int child_process(char *cmd, char **cmd_args, t_data *data, char **envp)
{
	if (!(data->fd < 0))
	{
		if (data->redirect_state == 1)
		{
			if (dup2(data->fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		if (data->redirect_state == 0)
		{
			if (dup2(data->fd, STDIN_FILENO) < 0)
				return (-1);
		}
	}
	ft_printf("proceding to execve: \n");
	if (manual_cmd(cmd_args, &data))
		return (0);
	else
		execve(cmd, cmd_args, envp);
	return (EXIT_SUCCESS);
}

static int parent_process(char *cmd, char **cmd_args, t_data *data, char **envp)
{
	int status;
	waitpid(-1, &status, 0);
	ft_printf("\033[0;92m %d getpid() --- %d pid.data\033[0;39m\n", getpid(), data->parent);
	return (status);
}

/*
/ *  
/ * TODO: 
/ * -	the child and the parent process are passing from the same 
/ * 	execution of the commands so the command get executed two times
/ * 
/ * -	the export command id hell on earth
*/

	// char **cmd_args;
	// char *tmp;
	// tmp = ft_strjoin_gnl(command[0], " ");
	// int i = 1;
	// while (command[i] && command[i][0])
	// {
	// 	ft_printf("%s\n", command[i]);
	// 	tmp = ft_strjoin_gnl(tmp, command[i]);
	// 	i++;
	// }
	// cmd_args = ft_split(tmp, 32);
	// Debug
	//printf("%s\n", cmd);
	//printf("%s\n", cmd_args[0]);
static void	execute_command(char **command, t_data *data, char **envp)
{
	char *cmd;
	pid_t parent;
	int status;

	cmd = NULL;
	cmd = find_cmd(command[0], data);
	data->parent = getpid();
	parent = fork();
	// ft_printf("%d\n", data->parent);
	if (parent < 0)
		exit(ft_printf("error with the fork"));
	//ft_printf("%d\n", status);
	if (!parent)
		child_process(cmd, command, data, envp);
	else
		status = parent_process(cmd, command, data, envp);
	//ft_printf("%d\n", status);
	// exit(1);
	return ;
}

static int find_redirect(t_token* head)
{
    t_token	*current = head;

    // Traverse the linked list
    while (current != NULL) {
        // Check if the current node's data is '>'
        if (current->type == 4) {
            printf("Found '>' character in the linked list.\n");
            // Uncomment below if you want to stop after finding the first '>'
            return (1);
        }
        // Move to the next node
        current = current->next;
    }
    printf("Finished searching.\n");
	return (0);
}

void	token_parser(t_token **tokens, t_data *data, char **envp)
{
	t_token		*current;
	t_token		*head;
	char		**command;
	int i = 0;
	// int *pipe;

	command = (char **)malloc(sizeof(char *) * 3);
	printf("starting parser: ------------------------->\n");
	current = *tokens;
	head = *tokens;
	while (current->type != TOKEN_EOF)
	{

		// Case for handling redirections
		while (current != NULL)
		{
			if (current->type == TOKEN_REDIRECT_OUT)
			{
				//printf("%s\n%s\n", current->value,"Found '>' character in the linked list.\n");
				current = current->next;
				printf("%s\n%d\n", current->value, current->type);
				data->redirect_state = 1;
				if (current->type == TOKEN_APPENDICE)
				{
					data->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
					printf("%d\n", data->fd);
				}
			}
			else if (current->type == TOKEN_REDIRECT_IN)
			{
				//printf("%s\n%s\n", current->value,"Found '<' character in the linked list.\n");
				current = current->next;
				printf("%s\n%d\n", current->value, current->type);
				data->redirect_state = 0;
				if (current->type == TOKEN_APPENDICE)
				{
					data->fd = open(current->value, O_RDONLY);
					printf("%d\n", data->fd);
				}
			}
			current = current->next;
		}

		current = head;
		
		if (current->type == 12)
		{
			command[i] = ft_strdup(current->value);
			i++;
			current = current->next;
			while (current->type == 13 || current->type == 1)
			{
				command[i] = ft_strdup(current->value);
				current = current->next;
				////////
				//Debug
				//printf("%s\n", command[i]);
				i++;
			}		
			// handle pipes and split the token list (?)
			// handle redirection before executing the command;
			ft_printf("executing command: ----------------------->\n");
			execute_command(command, data, envp);
			// exit(1);
			close(data->fd);
			return ;
			//current = current->next->next;
			//continue;
		}
		current = current->next;
	}
}