/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:31:42 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/21 22:31:43 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			if (!is_done_eating(data, i)) // if done eating detach the thread ?
			{
				pthread_mutex_lock(data->hungry_time_mutex);
				data->time_elapsed = getCurrentTime(&data->hungry_time[i]);
				pthread_mutex_unlock(data->hungry_time_mutex);
				if (!is_still_alive(data, i))
					/*pthread join to terminate the threads ? 5 800 300 200*/
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

	int philo_data[5]; //test with 200 philo
	data = (t_data *)(v_data);
	fill_philo_data(data, philo_data);
	printf("[%d]ms philo number %d is eating at the start\n",
			getCurrentTime(&data->currentTime),
			philo_data[PHILO_ID]);
	ft_usleep(data->time_to_sleep * 1000 * 0.2);
	while (philo_data[TIMES_EATING] != data->number_of_times_to_eat)
	{
		if (!take_fork_and_eat(data, philo_data))
			break ;
		if (!sleep_and_think(data, philo_data))
			break ;
	}
	if (philo_data[TIMES_EATING] == data->number_of_times_to_eat)
	{
		printf("!!!!!!!!!!!!philosopher %d is done eating!!!!!!!!!!!11\n",
				philo_data[INDEX] + 1);
		pthread_mutex_lock(data->done_eating_mutex);
		data->done_eating[philo_data[INDEX]] = 1;
		pthread_mutex_unlock(data->done_eating_mutex);
	}
	return (NULL);
}

void	*single_philo(t_data *data)
{
	pthread_mutex_lock(data->forkMutex);
	printf("[%d]ms philosophere 1 has locked left fork\n",
			getCurrentTime(&data->currentTime));
	ft_usleep(data->time_to_die * 1000);
	pthread_mutex_lock(&data->stop_simulation_mutex);
	data->stop_simulation = true;
	pthread_mutex_unlock(&data->stop_simulation_mutex);
	pthread_mutex_unlock(data->forkMutex);
	printf("[%d]ms philosopher 1 has died x_x\n",
			getCurrentTime(&data->currentTime));
	return (NULL);
}
void	*routine2(void *v_data)
{
	t_data	*data;

	int philo_data[5]; //test with 200 philo
	data = (t_data *)(v_data);
	fill_philo_data(data, philo_data);
	//pthread_cond_signal(&(data->iCond));
	if (data->philos == 1)
		return (single_philo(data));
	//printf("philsopher %d is not eating at the start\n", philo_id);
	while (data->number_of_times_to_eat != philo_data[TIMES_EATING])
	{
		if (!sleep_and_think(data, philo_data))
			break ;
		if (!take_fork_and_eat(data, philo_data))
			break ;
	}
	if (philo_data[TIMES_EATING] == data->number_of_times_to_eat)
	{
		printf("!!!!!!!!!!!!philosopher %d is done eating!!!!!!!!!!!11\n",
				philo_data[INDEX] + 1);
		pthread_mutex_lock(data->done_eating_mutex);
		data->done_eating[philo_data[INDEX]] = 1;
		pthread_mutex_unlock(data->done_eating_mutex);
	}
	return (NULL);
}
