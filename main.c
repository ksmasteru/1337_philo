/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <hes-saqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:45:05 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/23 11:03:40 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

int	init_mutexes(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philos)
		pthread_mutex_init(&(data->forkMutex[i++]), NULL);
	pthread_mutex_init(&(data->i_mutex), NULL);
	pthread_mutex_init(&(data->stop_simulation_mutex), NULL);
	pthread_mutex_init(&(data->number_of_meals_mutex), NULL);
	pthread_mutex_init(data->hungry_time_mutex, NULL);
	pthread_mutex_init(data->done_eating_mutex, NULL);
	return (0);
}

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos)
	{
		pthread_mutex_lock(&(data->i_mutex));
		data->philo_id = i;
		if (i % 2 == 1)
			pthread_create(data->ph_th + i, NULL, &routine1, (void *)data);
		else
			pthread_create(data->ph_th + i, NULL, &routine2, (void *)data);
		pthread_mutex_unlock(&(data->i_mutex));
		usleep(200);
		i++;
	}
	
	i = 0;
	pthread_create((data->monitor_thread), NULL, &monitor1, (void *)data);
	while (i < data->philos)
		pthread_join(data->ph_th[i++], NULL);
	pthread_join(*(data->monitor_thread), NULL);
	return (0);
}

void	free_data(t_data *data)
{
	free(data->done_eating);
	free(data->ph_th);
	free(data->monitor_thread);
	free(data->forkMutex);
	free(data->hungry_time);
	free(data->hungry_time_mutex);
	free(data->done_eating_mutex);
	free_list(data->a_head);
	free(data);
}

void	destroy_mutexes(t_data *data)
{
	int i;

	i = 0;
	pthread_mutex_destroy(&(data->i_mutex));
	pthread_mutex_destroy(data->hungry_time_mutex);
	pthread_mutex_destroy(&data->stop_simulation_mutex);
	pthread_mutex_destroy(&data->number_of_meals_mutex);
	pthread_mutex_destroy(data->done_eating_mutex);
	while (i < data->philos)
		pthread_mutex_destroy(&(data->forkMutex[i++]));
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->a_head = ft_parse(ac, av);
	if (!data->a_head)
	{
		free_list(data->a_head);
		free(data);
		return (1);
	}
	if (parse_data(data, av, ac) != 0)
	{
		free_list(data->a_head);
		free(data);
		return (1);
	}
	gettimeofday(&(data->currentTime), NULL);
	init_mutexes(data);
	create_threads(data);
	free_data(data);
	return (0);
}
