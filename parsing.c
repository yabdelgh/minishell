/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asfaihi <asfaihi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 17:31:31 by asfaihi           #+#    #+#             */
/*   Updated: 2021/06/19 19:00:35 by asfaihi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	env_var_parser(t_cmd *cmd, char *word)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	j = 0;
	while (word[i])
	{
		while (word[i] && word[i] != '$')
			i++;
		if (word[i++] == '$')
		{
			j = i;
			while (word[j] && word[j] != ' ' && word[j] != '$')
				j++;
			cmd->env_variable = ft_substr(word, i, j - 1);
			printf("\033[0;33mEnv Var: %s\033[0;0m\n", cmd->env_variable);
		}
		temp = cmd->env_variable;
		//free(temp);
	}
}

int	double_quotes(t_cmd *cmd, char *s, int i)
{
	int		x;

	x = 0;
	i++;
	while (s[i] != '"' && s[i])
		cmd->args[cmd->arg_num][x++] = s[i++];
	cmd->args[cmd->arg_num][x] = '\0';
	if (s[i] != '"')
	{
		printf("Unclosed double quote");
		exit(EXIT_FAILURE);
	}
	env_var_parser(cmd, cmd->args[cmd->arg_num]);
	cmd->arg_num++;
	return (i);
}

int	single_quotes(t_cmd *cmd, char *s, int i)
{
	int		x;

	x = 0;
	i++;
	while (s[i] != 39 && s[i])
		cmd->args[cmd->arg_num][x++] = s[i++];
	cmd->args[cmd->arg_num][x] = '\0';
	if (s[i] != 39)
	{
		printf("Unclosed single quote");
		exit(EXIT_FAILURE);
	}
	cmd->arg_num++;
	return (i);
}

void	get_file(t_redirect *redirect, char *s, int i)
{
	int		j;

	while (s[i] == ' ')
		i++;
	j = i;
	while (s[i] && s[i] != ' ' && s[i] != '>' && s[i] != '<')
	{
		i++;
	}
	redirect->file = ft_substr(s, j, i);
}

t_redirect	*redirections(t_cmd *new, char *s, int i)
{
	t_redirect	*redirect;

	if (!(redirect = (t_redirect *)malloc(sizeof(t_redirect))))
		return (NULL);
	if (s[i] == '>' && s[i + 1] != '>')
	{
		redirect->type = 'G';
		printf("--%c--", redirect->type);
		get_file(redirect, s, i + 1);
	}
	else if (s[i] == '<' && s[i + 1] != '<')
	{
		redirect->type = 'L';
		printf("--%c--", redirect->type);
		get_file(redirect, s, i + 1);
	}
	else if (s[i] == '>' && s[i + 1] == '>')
	{
		redirect->type = 'D';
		printf("--%c--", redirect->type);
		get_file(redirect, s, i + 2);
	}
	else if (s[i] == '<' && s[i + 1] == '<')
	{
		redirect->type = 'H';
		printf("--%c--", redirect->type);
		get_file(redirect, s, i + 2);
	}
	printf("(%s)", redirect->file);
}

void	get_args(t_cmd *new, char *s)
{
	int			i;
	int			j;
	t_redirect	*redirect;
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (s[i] == '<' || s[i] == '>')
		{
			redirect = redirections(new, s, i);
			break;
		}
		if (s[i] && s[i] != '"' && s[i] != 39)
		{
			j = 0;
			while (s[i] != ' ' && s[i] != '"' && s[i] != 39
				&& s[i] && s[i] != '<' && s[i]!= '>')
				new->args[new->arg_num][j++] = s[i++];
			new->args[new->arg_num][j] = '\0';
			env_var_parser(new, new->args[new->arg_num]);
			new->arg_num++;
		}
		if (s[i] == '"')
			i = double_quotes(new, s, i);
		if (s[i] == 39)
			i = single_quotes(new, s, i);
		if (s[i])
			i++;
	}
}

t_cmd	*new_node(char *s)
{
	t_cmd	*new;
	int		i;
	int		x;

	if (!(new = (t_cmd *)malloc(sizeof(t_cmd))))
		return (NULL);
	initialize_node(new, s);
	i = 0;
	x = 0;
	get_args(new, s);
	new->cmd = new->args[0];
	return (new);
}