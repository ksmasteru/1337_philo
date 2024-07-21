void *routine1(void  *v_data)
{
  int philo_data[5]; //test with 200 philo
  t_data *data;

  data = (t_data *)(v_data);
  fill_philo_data(data, philo_data);
  printf("[%d]ms philo number %d is eating at the start\n", getCurrentTime(&data->currentTime), philo_data[PHILO_ID]);
  if (philo_take_fork(data, philo_data[INDEX], philo_data[LEFT_FORK], philo_data[RIGHT_FORK]) < 0) // less than zero if stop_simulation is true
    return NULL;
  if (philo_think(data, philo_data[INDEX]) < 0) /* a little delay*/
  {
    pthread_mutex_unlock(data->forkMutex + philo_data[LEFT_FORK] - 1);
    pthread_mutex_unlock(data->forkMutex + philo_data[RIGHT_FORK] - 1);
    return NULL;
  }
  //delat thinking so the pohilosohers uses some oh his alive time
  ft_usleep(data->time_to_die * 1000 * 0.2);
  if (philo_eat(data, philo_data[INDEX]) < 0)
  {
    pthread_mutex_unlock(data->forkMutex + philo_data[LEFT_FORK] - 1);
    pthread_mutex_unlock(data->forkMutex + philo_data[RIGHT_FORK] - 1);
    return NULL;
  }
  else
    philo_data[TIMES_EATING]++;
  pthread_mutex_unlock(data->forkMutex + philo_data[LEFT_FORK] - 1);
  pthread_mutex_unlock(data->forkMutex + philo_data[RIGHT_FORK] - 1);
  pthread_cond_signal(data->forkCond + philo_data[RIGHT_FORK] - 1);
  if (philo_sleep(data, philo_data[INDEX]) < 0)
    return (NULL);
  while (philo_data[TIMES_EATING] != data->number_of_times_to_eat)
  {
    if (!take_fork_and_eat(data, philo_data))
      break;
    if (!sleep_and_think(data, philo_data))
      break;
  }
  if (philo_data[TIMES_EATING] == data->number_of_times_to_eat)
  {
    printf("!!!!!!!!!!!!philosopher %d is done eating!!!!!!!!!!!11\n", philo_data[INDEX] + 1);
    pthread_mutex_lock(data->done_eating_mutex);
    data->done_eating[philo_data[INDEX]] = 1;
    pthread_mutex_unlock(data->done_eating_mutex);
  }
  return (NULL);
}