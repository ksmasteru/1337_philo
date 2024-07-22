/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <hes-saqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:31:16 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/22 20:14:29 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

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

int	parse_data(t_data *data, char **av, int ac)
{
	int	i;

	i = -1;
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

int	get_philo_args(t_data *data, char **av, int ac)
{
	data->stop_simulation = false;
	data->philos = ft_atoi(av[1]);
	if (data->philos < 0)
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
	data->time_elapsed = 0;
	if (data->time_elapsed < 0)
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
	data->hungry_time_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_t));
	if (!data->hungry_time_mutex)
		return (1);
	data->done_eating = (int *)malloc(sizeof(int) * data->philos);
	if (!data->done_eating)
		return (1);
	return (0);
}
