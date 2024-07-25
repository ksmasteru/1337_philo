/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:45:41 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/25 19:45:45 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo(t_data *data, int left_fork)
{
	pthread_mutex_lock(data->forkmutex + left_fork - 1);
	pthread_mutex_lock(&data->get_current_time_mutex);
	printf("%d philosopher number 1 has taken a fork\n",
		getcurrenttime(&(data->currenttime)));
	pthread_mutex_unlock(&data->get_current_time_mutex);
	pthread_mutex_unlock(data->forkmutex + left_fork - 1);
	ft_usleep(data->time_to_die * 2000);
	return (NULL);
}

void	fill_philo_data(t_data *data, int *philo_data)
{
	int	swap;

	pthread_mutex_lock(&(data->i_mutex));
	philo_data[INDEX] = data->philo_id;
	philo_data[PHILO_ID] = 1 + data->philo_id--;
	philo_data[RIGHT_FORK] = philo_data[PHILO_ID];
	philo_data[TIMES_EATING] = 0;
	if (data->philos == philo_data[PHILO_ID])
		philo_data[LEFT_FORK] = 1;
	else
		philo_data[LEFT_FORK] = philo_data[RIGHT_FORK] + 1;
	if (data->philos % 2 == 0 && (data->philos == philo_data[PHILO_ID]))
	{
		swap = philo_data[LEFT_FORK];
		philo_data[LEFT_FORK] = philo_data[RIGHT_FORK];
		philo_data[RIGHT_FORK] = swap;
	}
	pthread_mutex_unlock(&(data->i_mutex));
}
