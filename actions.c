/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <hes-saqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:30:04 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/24 21:13:21 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"


int one_philo(t_data *data, int left_fork)
{
	pthread_mutex_lock(data->forkMutex + left_fork - 1);
	pthread_mutex_lock(&data->get_current_time_mutex);
	printf("%d philosopher number 1 has taken a fork\n", getCurrentTime(&(data->currentTime)));
	pthread_mutex_unlock(&data->get_current_time_mutex);
    pthread_mutex_unlock(data->forkMutex + left_fork - 1);
    ft_usleep(data->time_to_die * 2000);
	return (-1);
}

int print_message(t_data *data, int index, int msg_index)
{

	pthread_mutex_lock(&data->get_current_time_mutex);
	pthread_mutex_lock(&data->stop_simulation_mutex);
	if (data->stop_simulation == true)
	{
		pthread_mutex_unlock(&data->stop_simulation_mutex);
		pthread_mutex_unlock(&data->get_current_time_mutex);
		return (-1);
	}
	if (msg_index == 0)
		printf("%d philosopher number %d has taken a fork\n",
			getCurrentTime(&data->currentTime),
			index + 1);
	else if (msg_index == 1)
	{
		printf("%d philosopher number %d is eating\n",
			getCurrentTime(&(data->currentTime)), index + 1);
	}
	else if (msg_index == 2)
		printf("%d philosopher number %d is thinking\n", getCurrentTime(&(data->currentTime)),index + 1);
	else if (msg_index == 3)
		printf("%d philosopher number %d is sleeping\n", getCurrentTime(&(data->currentTime)), index + 1);
	pthread_mutex_unlock(&data->stop_simulation_mutex);
	pthread_mutex_unlock(&data->get_current_time_mutex);
	return (0);
}

int	philo_take_fork(t_data *data, int index, int left_fork, int right_fork)
{
	if (data->philos == 1)
		return (one_philo(data ,left_fork));
	pthread_mutex_lock(data->forkMutex + left_fork - 1);
	if (print_message(data, index, 0) != 0)
	{
		pthread_mutex_unlock(data->forkMutex + left_fork - 1);
		return (-1);
	}
	pthread_mutex_lock(data->forkMutex + right_fork - 1);
	if (print_message(data, index, 0) != 0)
	{
		pthread_mutex_unlock(data->forkMutex + right_fork - 1);
		return (-1);
	}
	return (0);
}

int	philo_eat(t_data *data, int index)
{
	pthread_mutex_lock(data->hungry_time_mutex);
	gettimeofday(&(data->hungry_time[index]), NULL);
	pthread_mutex_unlock(data->hungry_time_mutex);
	if (print_message(data, index, 1) != 0)
		return (-1);
	ft_usleep(data->time_to_eat * 1000);
	return (0);
}

int	philo_think(t_data *data, int index)
{
	if (print_message(data, index , 2) < 0)
		return (-1);
	return (0);
}

int	philo_sleep(t_data *data, int index)
{
	if (print_message(data, index , 3) < 0)
		return (-1);
	else
	ft_usleep(data->time_to_sleep * 1000);
	return (0);
}
