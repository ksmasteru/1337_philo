/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <hes-saqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:31:42 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/24 20:00:12 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

void	*monitor1(void *v_data)
{
	t_data	*data;
	int		i;
	data = (t_data *)v_data;
	i = 0;
	set_hungry_time(data);
	while (1)
	{
		i = 0;
		if (check_number_meals(data))
			return (NULL);
		while (i < data->philos)
		{
			if (!is_done_eating(data, i))
			{
				pthread_mutex_lock(data->hungry_time_mutex);
				data->time_elapsed = getCurrentTime(&data->hungry_time[i]);
				pthread_mutex_unlock(data->hungry_time_mutex);
				if (is_still_alive(data, i) == 1)
					return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}

void	*routine1(void *v_data)
{
	t_data	*data;

	int philo_data[5];
	data = (t_data *)(v_data);
	fill_philo_data(data, philo_data);
	while (philo_data[TIMES_EATING] != data->number_of_times_to_eat)
	{
		if (!take_fork_and_eat(data, philo_data))
			break ;
		if (!sleep_and_think(data, philo_data))
			break ;
	}
	if (data->philos > 1 && philo_data[TIMES_EATING] == data->number_of_times_to_eat)
	{
		pthread_mutex_lock(data->done_eating_mutex);
		data->done_eating[philo_data[INDEX]] = 1;
		pthread_mutex_unlock(data->done_eating_mutex);
	}
	return (NULL);
}

void	*single_philo(t_data *data)
{
	pthread_mutex_lock(data->forkMutex);
	printf("%d philosopher number 1 has taken a fork\n",
			getCurrentTime(&data->currentTime));
	ft_usleep(data->time_to_die * 1000);
	pthread_mutex_lock(&data->stop_simulation_mutex);
	data->stop_simulation = true;
	pthread_mutex_unlock(&data->stop_simulation_mutex);
	pthread_mutex_unlock(data->forkMutex);
	return (NULL);
}
void	*routine2(void *v_data)
{
	t_data	*data;

	int philo_data[5];
	data = (t_data *)(v_data);
	fill_philo_data(data, philo_data);
	ft_usleep(data->time_to_sleep * 1000 * 0.2);
	while (data->number_of_times_to_eat != philo_data[TIMES_EATING])
	{
		if (!take_fork_and_eat(data, philo_data))
			break ;
		if (!sleep_and_think(data, philo_data))
			break ;
	}
	if (data->philos > 1 && philo_data[TIMES_EATING] == data->number_of_times_to_eat)
	{
		pthread_mutex_lock(data->done_eating_mutex);
		data->done_eating[philo_data[INDEX]] = 1;
		pthread_mutex_unlock(data->done_eating_mutex);
	}
	return (NULL);
}
void	*routine3(void *v_data)
{
	t_data	*data;

	int philo_data[5];
	data = (t_data *)(v_data);
	fill_philo_data(data, philo_data);
	while (philo_data[TIMES_EATING] != data->number_of_times_to_eat)
	{
		if (!take_fork_and_eat(data, philo_data))
			break ;
		if (!sleep_and_think(data, philo_data))
			break ;
	}
	if (data->philos > 1 && philo_data[TIMES_EATING] == data->number_of_times_to_eat)
	{
		pthread_mutex_lock(data->done_eating_mutex);
		data->done_eating[philo_data[INDEX]] = 1;
		pthread_mutex_unlock(data->done_eating_mutex);
	}
	return (NULL);
}

void	*routine4(void *v_data)
{
	t_data	*data;

	int philo_data[5];
	data = (t_data *)(v_data);
	fill_philo_data(data, philo_data);
	while (philo_data[TIMES_EATING] != data->number_of_times_to_eat)
	{
		if (!sleep_and_think(data, philo_data))
			break ;
		if (!take_fork_and_eat(data, philo_data))
			break ;
	}
	if (data->philos > 1 && philo_data[TIMES_EATING] == data->number_of_times_to_eat)
	{
		pthread_mutex_lock(data->done_eating_mutex);
		data->done_eating[philo_data[INDEX]] = 1;
		pthread_mutex_unlock(data->done_eating_mutex);
	}
	return (NULL);
}
