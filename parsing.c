/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:35:15 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/25 19:36:18 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	link_numbers(char **num_str, int x, t_stack **tmp, t_stack **head)
{
	long	number;
	int		j;
	t_stack	*new;

	j = 0;
	number = 0;
	while (num_str[j] != NULL)
	{
		number = ft_atoi(num_str[j]);
		if (number > INT_MAX || number < INT_MIN)
			return (-1);
		if (x == 0)
		{
			*tmp = ft_lst_new(number);
			*head = *tmp;
			x++;
			j++;
			continue ;
		}
		new = ft_lstadd_back(*tmp, number);
		*tmp = new;
		j++;
	}
	return (0);
}

t_stack	*ft_parse(int ac, char **av)
{
	t_stack	*a_head;
	t_stack	*tmp;
	char	**num_str;
	int		i;
	int		x;

	x = 0;
	i = 0;
	a_head = NULL;
	tmp = a_head;
	while (i < ac - 1)
	{
		num_str = ft_split(av[i + 1], 32);
		if (num_str == NULL)
			return (NULL);
		if (link_numbers(num_str, x++, &tmp, &a_head) < 0)
		{
			free_2d_str(num_str);
			return (NULL);
		}
		free_2d_str(num_str);
		i++;
	}
	return (a_head);
}

int	parse_data(t_data *data)
{
	int	i;

	i = -1;
	data->number_of_times_to_eat = -1;
	if (das_parsing(data) != 0)
		return (1);
	data->stop_simulation = false;
	data->time_elapsed = 0;
	if (data->time_elapsed < 0)
		return (1);
	if (!((stack_len(data->a_head) == 4 || stack_len(data->a_head) == 5)))
		return (1);
	if (data_mem_alloc(data) != 0)
		return (1);
	while (++i < data->philos)
		data->done_eating[i] = 0;
	return (0);
}

int	das_parsing(t_data *data)
{
	int		i;
	t_stack	*tmp;

	tmp = data->a_head;
	i = 0;
	while (tmp)
	{
		if (tmp->data < 0)
			return (1);
		if (i == 0)
			data->philos = tmp->data;
		else if (i == 1)
			data->time_to_die = tmp->data;
		else if (i == 2)
			data->time_to_eat = tmp->data;
		else if (i == 3)
			data->time_to_sleep = tmp->data;
		else if (i == 4)
			data->number_of_times_to_eat = tmp->data;
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int	data_mem_alloc(t_data *data)
{
	data->ph_th = (pthread_t *)malloc(sizeof(pthread_t) * data->philos);
	if (!data->ph_th)
		return (1);
	data->hungry_time = (struct timeval *)malloc(sizeof(struct timeval)
			* data->philos);
	data->forkmutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->philos);
	if (!data->forkmutex)
		return (1);
	data->done_eating_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->philos);
	if (!data->done_eating_mutex)
		return (1);
	data->monitor_thread = (pthread_t *)malloc(sizeof(pthread_t));
	if (!data->monitor_thread)
		return (1);
	data->hungry_time_mutex = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t));
	if (!data->hungry_time_mutex)
		return (1);
	data->done_eating = (int *)malloc(sizeof(int) * data->philos);
	if (!data->done_eating)
		return (1);
	return (0);
}
