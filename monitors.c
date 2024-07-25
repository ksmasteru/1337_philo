/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 20:06:35 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/25 20:06:37 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_number_meals(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos)
	{
		pthread_mutex_lock(data->done_eating_mutex);
		if (data->done_eating[i++] == 0)
		{
			pthread_mutex_unlock(data->done_eating_mutex);
			return (false);
		}
		pthread_mutex_unlock(data->done_eating_mutex);
	}
	return (true);
}

void	set_hungry_time(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(data->hungry_time_mutex);
	while (i < data->philos)
		gettimeofday(&(data->hungry_time[i++]), NULL);
	pthread_mutex_unlock(data->hungry_time_mutex);
}

bool	is_done_eating(t_data *data, int index)
{
	pthread_mutex_lock(data->done_eating_mutex);
	if (data->done_eating[index] == 1)
	{
		pthread_mutex_unlock(data->done_eating_mutex);
		return (true);
	}
	pthread_mutex_unlock(data->done_eating_mutex);
	return (false);
}

int	is_still_alive(t_data *data, int index)
{
	pthread_mutex_lock(data->hungry_time_mutex);
	pthread_mutex_lock(&data->get_current_time_mutex);
	pthread_mutex_lock(&data->stop_simulation_mutex);
	data->time_elapsed = getcurrenttime(&data->hungry_time[index]);
	if (data->time_elapsed > data->time_to_die)
	{
		data->stop_simulation = true;
		printf("%d philosopher number %d is dead\n",
			getcurrenttime(&data->hungry_time[index]), index + 1);
		pthread_mutex_unlock(&data->stop_simulation_mutex);
		pthread_mutex_unlock(&data->get_current_time_mutex);
		pthread_mutex_unlock(data->hungry_time_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->stop_simulation_mutex);
	pthread_mutex_unlock(&data->get_current_time_mutex);
	pthread_mutex_unlock(data->hungry_time_mutex);
	return (0);
}

int	getcurrenttime(struct timeval *currenttime)
{
	struct timeval	elapsedtime;

	if (!currenttime)
		return (-1);
	gettimeofday(&elapsedtime, NULL);
	return ((elapsedtime.tv_sec * 1000 + elapsedtime.tv_usec / 1000)
		- ((currenttime->tv_sec) * 1000 + (currenttime->tv_usec) / 1000));
}
