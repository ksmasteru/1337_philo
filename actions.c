/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 20:06:19 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/25 20:06:21 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_message(t_data *data, int index, int msg_index)
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
			getcurrenttime(&data->currenttime), index + 1);
	else if (msg_index == 1)
	{
		printf("%d philosopher number %d is eating\n",
			getcurrenttime(&(data->currenttime)), index + 1);
	}
	else if (msg_index == 2)
		printf("%d philosopher number %d is thinking\n",
			getcurrenttime(&(data->currenttime)), index + 1);
	else if (msg_index == 3)
		printf("%d philosopher number %d is sleeping\n",
			getcurrenttime(&(data->currenttime)), index + 1);
	pthread_mutex_unlock(&data->stop_simulation_mutex);
	pthread_mutex_unlock(&data->get_current_time_mutex);
	return (0);
}

int	philo_take_fork(t_data *data, int index, int left_fork, int right_fork)
{
	pthread_mutex_lock(data->forkmutex + left_fork - 1);
	if (print_message(data, index, 0) != 0)
	{
		pthread_mutex_unlock(data->forkmutex + left_fork - 1);
		return (-1);
	}
	pthread_mutex_lock(data->forkmutex + right_fork - 1);
	if (print_message(data, index, 0) != 0)
	{
		pthread_mutex_unlock(data->forkmutex + left_fork - 1);
		pthread_mutex_unlock(data->forkmutex + right_fork - 1);
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
	if (print_message(data, index, 2) < 0)
		return (-1);
	return (0);
}

int	philo_sleep(t_data *data, int index)
{
	if (print_message(data, index, 3) < 0)
		return (-1);
	else
		ft_usleep(data->time_to_sleep * 1000);
	return (0);
}
