/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <hes-saqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:31:16 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/23 11:41:54 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

void	fill_philo_data(t_data *data, int *philo_data)
{
	pthread_mutex_lock(&(data->i_mutex));
	philo_data[INDEX] = data->philo_id;
	philo_data[PHILO_ID] = 1 + data->philo_id--;
	philo_data[RIGHT_FORK] = philo_data[PHILO_ID];
	philo_data[TIMES_EATING] = 0;
	if (data->philos == philo_data[PHILO_ID])
		philo_data[LEFT_FORK] = 1;
	else
		philo_data[LEFT_FORK] = philo_data[RIGHT_FORK] + 1;
	pthread_mutex_unlock(&(data->i_mutex));
}

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

int	parse_data(t_data *data, char **av, int ac)
{
	int i;
	
	i = -1;
	data->a_head = ft_parse(ac, av);
	if (!data->a_head)
	{
		free_lst(data->a_head);
		free(data);
		return (1);
	}
	if (get_philo_args(data, av, ac) != 0)
	{
		free(data);
		return (1);
	}
	if (data_mem_alloc(data) != 0)
	{
		free(data);
		return (1);
	}
	while (++i < data->philos)
		data->done_eating[i] = 0;
	return (0);
}

// stopped here
int das_parsing(t_data *data)
{
	int i;
	t_stack *tmp;

	tmp = data->a_head;
	i = 0;
	while (tmp)
	{
		if (i == 0)
			data->philos = tmp->data;
		else if (i == 1)
			data->time_to_die = tmp->data;
		else if (i == 2)
			data->time_to_eat = tmp->data;
		else if (i == 3)
			data->time_to_sleep = tmp->data;
		else if (i == 4)
			data
	}
}
int	get_philo_args(t_data *data, char **av, int ac)
{
	data->stop_simulation = false;
	data->time_elapsed = 0; // ???
	if (data->time_elapsed < 0)
		return (1);
	if (!((stack_len(data->a_head) == 4 || stack_len(data->a_head) == 5)))
	{
		printf("not enough args\n");
		return (1);
	}
	data->philos = ft_atoi(av[1]);
	if (data->philos <= 0)
		return (1);
	data->time_to_die = ft_atoi(av[2]);
	if (data->time_to_die < 0 || data->time_to_die > INT_MAX)
		return (1);
	data->time_to_eat = ft_atoi(av[3]);
	if (data->time_to_eat < 0 || data->time_to_eat > INT_MAX)
		return (1);
	data->time_to_sleep = ft_atoi(av[4]);
	if (data->time_to_sleep < 0 || data->time_to_sleep > INT_MAX)
		return (1);

	if (ac == 6)
	{
		data->number_of_times_to_eat = ft_atoi(av[5]);
		if (data->number_of_times_to_eat <= 0
			|| data->number_of_times_to_eat > INT_MAX)
			return (1);
	}
	else
		data->number_of_times_to_eat = -1;
	return (0);
}

int	data_mem_alloc(t_data *data)
{
	data->ph_th = (pthread_t *)malloc(sizeof(pthread_t) * data->philos);
	if (!data->ph_th)
		return (1);
	data->hungry_time = (struct timeval *)malloc(sizeof(struct timeval)
			* data->philos);
	if (!data->hungry_time)
		return (1);
	data->forkMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->philos);
	if (!data->forkMutex)
		return (1);
	data->done_eating_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->philos);
	if (!data->done_eating_mutex)
		return (1);
	data->monitor_thread = (pthread_t *)malloc(sizeof(pthread_t));
	if (!data->monitor_thread)
		return (1);
	data->hungry_time_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!data->hungry_time_mutex)
		return (1);
	data->done_eating = (int *)malloc(sizeof(int) * data->philos);
	if (!data->done_eating)
		return (1);
	return (0);
}
