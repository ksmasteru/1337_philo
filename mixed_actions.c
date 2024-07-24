/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixed_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <hes-saqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:30:52 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/24 12:58:59 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

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
	if (philo_take_fork(data, philo_data[INDEX], philo_data[LEFT_FORK], philo_data[RIGHT_FORK]) < 0)
		return (false);
	if (philo_eat(data, philo_data[INDEX]) < 0)
	{
		pthread_mutex_unlock(data->forkMutex + philo_data[LEFT_FORK] - 1);
		pthread_mutex_unlock(data->forkMutex + philo_data[RIGHT_FORK] - 1);
		return (false);
	}
	else
		philo_data[TIMES_EATING]++;
	pthread_mutex_unlock(data->forkMutex + philo_data[LEFT_FORK] - 1);
	pthread_mutex_unlock(data->forkMutex + philo_data[RIGHT_FORK] - 1);
	return (true);
}
