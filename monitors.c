/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <hes-saqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:31:05 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/22 18:41:22 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

bool	check_number_meals(t_data *data)
{
	int	i;
	int	j;

	j = 0;
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
		printf("stuck");
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
	data->time_elapsed = getCurrentTime(&data->hungry_time[index]);
	pthread_mutex_unlock(data->hungry_time_mutex);
	if (data->time_elapsed > data->time_to_die)
	{
		pthread_mutex_lock(&(data->stop_simulation_mutex));
		data->stop_simulation = true;
		printf("--------[%d]ms philospher % d dead detected by the monitor !!!!-------\n ",getCurrentTime(&data->hungry_time[index]), index + 1);
		pthread_mutex_unlock(&(data->stop_simulation_mutex));
		return (1);
	}
	return (0);
}

int	getCurrentTime(struct timeval *currentTime)
{
	struct timeval	elapsedTime;

	if (!currentTime)
		return (-1);
	gettimeofday(&elapsedTime, NULL);
	return (elapsedTime.tv_sec * 1000 + elapsedTime.tv_usec / 1000)
	- ((currentTime->tv_sec) * 1000 + (currentTime->tv_usec) / 1000);
}
