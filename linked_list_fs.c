/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_fs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:31:48 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/25 19:31:52 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_list(t_stack *head)
{
	t_stack	*tmp;
	t_stack	*nexto;

	tmp = head;
	while (tmp != NULL)
	{
		nexto = tmp->next;
		free(tmp);
		tmp = nexto;
	}
}

void	free_2d_str(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str[i]);
	free(str);
}

t_stack	*ft_lst_new(int data)
{
	t_stack	*new;

	new = malloc(sizeof(t_stack));
	if (!new)
		return (NULL);
	new->data = data;
	new->next = NULL;
	return (new);
}

t_stack	*ft_lstadd_back(t_stack *old, int data)
{
	t_stack	*new;

	new = ft_lst_new(data);
	if (!new)
		return (NULL);
	old->next = new;
	return (new);
}

int	stack_len(t_stack *head)
{
	int		i;
	t_stack	*tmp;

	i = 0;
	tmp = head;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}
