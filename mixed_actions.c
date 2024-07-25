/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixed_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:33:05 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/25 19:33:20 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos)
		pthread_mutex_init(&(data->forkmutex[i++]), NULL);
	pthread_mutex_init(&(data->i_mutex), NULL);
	pthread_mutex_init(&(data->stop_simulation_mutex), NULL);
	pthread_mutex_init(&(data->number_of_meals_mutex), NULL);
	pthread_mutex_init(data->hungry_time_mutex, NULL);
	pthread_mutex_init(data->done_eating_mutex, NULL);
	pthread_mutex_init(&(data->get_current_time_mutex), NULL);
	return (0);
}

int	ft_usleep(double sec)
{
	int	i;

	i = 0;
	while (i++ < 10)
		usleep(sec / 10);
	return (0);
}

bool	sleep_and_think(t_data *data, int *philo_data)
{
	if (philo_sleep(data, philo_data[INDEX]) < 0)
		return (false);
	if (philo_think(data, philo_data[INDEX]) < 0)
		return (false);
	return (true);
}

bool	take_fork_and_eat(t_data *data, int *philo_data)
{
	if (philo_take_fork(data, philo_data[INDEX], philo_data[LEFT_FORK],
			philo_data[RIGHT_FORK]) < 0)
		return (false);
	if (philo_eat(data, philo_data[INDEX]) < 0)
	{
		pthread_mutex_unlock(data->forkmutex + philo_data[LEFT_FORK] - 1);
		pthread_mutex_unlock(data->forkmutex + philo_data[RIGHT_FORK] - 1);
		return (false);
	}
	else
		philo_data[TIMES_EATING]++;
	pthread_mutex_unlock(data->forkmutex + philo_data[LEFT_FORK] - 1);
	pthread_mutex_unlock(data->forkmutex + philo_data[RIGHT_FORK] - 1);
	return (true);
}
