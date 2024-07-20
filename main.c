#include "philo.h"

void parse_data(t_data *data, char **av, int ac)
{
  int i;

  i = -1;
  data->stop_simulation = false;
  data->philos = atoi(av[1]);
  data->time_to_die = atoi(av[2]);
  data->time_to_eat = atoi(av[3]);
  data->time_to_sleep = atoi(av[4]);
  data->available_forks = data->philos;
  if (ac == 6)
  {
    data->number_of_times_to_eat = atoi(av[5]);
    printf("number of times philo should eat is %d\n", data->number_of_times_to_eat);
  }
  data->ph_th = (pthread_t *) malloc(sizeof(pthread_t) * data->philos);
  data->hungry_time = (struct timeval *) malloc(sizeof(struct timeval) * data->philos);
  data->forkMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->philos);
  data->forkCond = (pthread_cond_t *) malloc(sizeof(pthread_cond_t) * data->philos);
  data->number_of_meals = (int *)malloc(sizeof(int) * data->philos);
  data->monitor_thread = (pthread_t *)malloc(sizeof(pthread_t));
  while (++i < data->philos)
  {
    pthread_mutex_init(&(data->forkMutex[i]), NULL); 
    pthread_cond_init(&(data->forkCond[i]),NULL);
    data->number_of_meals[i] = 0;
  }
}

/* 
  returns elapsed time in ms since the initialiaztion of the struct parameter
*/
int getCurrentTime(struct timeval *currentTime)
{
  struct timeval elapsedTime;
  gettimeofday(&elapsedTime, NULL);
  return (elapsedTime.tv_sec * 1000 + elapsedTime.tv_usec / 1000) - ((currentTime->tv_sec) * 1000 + (currentTime->tv_usec) / 1000);
}

void *routine1(void  *v_data)
{
  int philo_id;
  int left_fork;
  int right_fork;
  int index;

  t_data *data = (t_data *)(v_data);
  pthread_mutex_lock(&(data->i_mutex));
  index = data->philo_id;
  philo_id = 1 + data->philo_id--;
  right_fork = philo_id;
  if (data->philos == philo_id)
    left_fork = 1;
  else
    left_fork = right_fork + 1;
  pthread_mutex_unlock(&(data->i_mutex));
  pthread_cond_signal(&(data->iCond));
  printf("[%d]ms philo number %d is eating at the start\n", getCurrentTime(&data->currentTime), philo_id);
  //gettimeofday(&(data->hungry_time[index]), NULL);
  while (getCurrentTime(&(data->hungry_time[index])) <= data->time_to_die)
  {
    //data->available_forks -= 1;
    if (philo_take_fork(data, index, left_fork, right_fork) < 0) // less than zero if stop_simulation is true
      break;
    if (philo_eat(data, index) < 0)
    {
      pthread_mutex_unlock(data->forkMutex + left_fork - 1);
      pthread_mutex_unlock(data->forkMutex + right_fork - 1);
      break;
    }
    /*{
      pthread_mutex_unlock(data->forkMutex + left_fork - 1);
      pthread_mutex_unlock(data->forkMutex + right_fork - 1);
      printf("-----------philosopher %d died x_x------------\n", philo_id);
      break;
    }*/
    pthread_mutex_unlock(data->forkMutex + left_fork - 1);
    pthread_mutex_unlock(data->forkMutex + right_fork - 1);
    //data->available_forks += 1;
   // pthread_mutex_unlock(data->forkMutex + left_fork - 1);
    // should notify thinking pihlosoers that the for has
    pthread_cond_signal(data->forkCond + right_fork - 1);
    //data->available_forks += 1;
    pthread_mutex_unlock(data->forkMutex + right_fork - 1);
    //pthread_cond_signal(data->forkCond + right_fork);
    if (philo_sleep(data, index) < 0)
      break;
    if (philo_think(data, index) < 0)
      break;
  //pthread_mutex_unlock (&(data->forkMutex));
  }
  printf("----------[%d]ms philosopher %d died x_x----------\n",getCurrentTime(&data->currentTime), index);
  return (NULL);
}

void *routine2(void  *v_data)
{
  int philo_id;
  int left_fork;
  int right_fork;
  int index;

  t_data *data = (t_data *)(v_data);
  pthread_mutex_lock(&(data->i_mutex));
  index = data->philo_id;
  philo_id = 1 + data->philo_id--;
  right_fork = philo_id;
  if (data->philos == philo_id)
    left_fork = 1;
  else
    left_fork = right_fork + 1;
  pthread_mutex_unlock(&(data->i_mutex));
  pthread_cond_signal(&(data->iCond));
  //gettimeofday(&(data->hungry_time[index]), NULL);
  if (data->philos  == 1)
  {
    pthread_mutex_lock(data->forkMutex);
    printf("[%d]ms philosophere 1 has locked left fork\n", getCurrentTime(&data->currentTime));
    usleep(data->time_to_die * 1000);
    pthread_mutex_lock(&data->stop_simulation_mutex);
    data->stop_simulation = true;
    pthread_mutex_unlock(&data->stop_simulation_mutex);
    pthread_mutex_unlock(data->forkMutex);
    printf("[%d]ms philosopher 1 has died x_x\n", getCurrentTime(&data->currentTime));
    return (NULL);
  }
    // at first run they shouldnt // be the firs to pick the fork.
    //getCurrentTime(&(data->hungry_time[index])) <= data->time_to_die)
  printf("philsopher %d is not eating at the start\n", philo_id);
  if (philo_sleep(data, index) < 0)
    return (NULL);
  if (philo_think(data, index) < 0)
    return (NULL);
  if (philo_take_fork(data, index, left_fork, right_fork) < 0)
    return (NULL);
  //if (getCurrentTime(&(data->hungry_time[index])) < data->time_to_die) :: monitor handled
  if (philo_eat(data, index) < 0)
  {
    pthread_mutex_unlock(data->forkMutex + left_fork - 1);
    pthread_mutex_unlock(data->forkMutex + right_fork - 1);
    return (NULL);
  }
  /*else
    { 
      pthread_mutex_unlock(data->forkMutex + left_fork - 1);
      pthread_mutex_unlock(data->forkMutex + right_fork - 1);
      printf("-----------philosopher %d died x_x------------\n", philo_id);
      return (NULL);
    }*/
  pthread_mutex_unlock(data->forkMutex + left_fork - 1);
  pthread_mutex_unlock(data->forkMutex + right_fork - 1);
  if (philo_sleep(data, index) < 0)
    return (NULL);
  if (philo_think(data, index) < 0)
    return (NULL);
  while (getCurrentTime(&(data->hungry_time[index])) < data->time_to_die)
  {
    if (philo_take_fork(data, index, left_fork, right_fork) < 0)
      break;
    if (philo_eat(data, index) < 0)
    {
      pthread_mutex_unlock(data->forkMutex + left_fork - 1);
      pthread_mutex_unlock(data->forkMutex + right_fork - 1);
      break;
    }
    pthread_mutex_unlock(data->forkMutex + left_fork - 1);
    pthread_mutex_unlock(data->forkMutex + right_fork - 1);
    if (philo_sleep(data, index) < 0)
      break;
    if (philo_think(data, index) < 0)
      break;
  }
  // lock left lock right eat put sleep think repeat
  printf("-----------philosopher %d died x_x------------\n", philo_id);
  return (NULL);
}

/* return 0 if each philos has at least eaten number_of_times_to_Eat
can this be done by each philosopher ?*/
int check_number_meals(t_data *data)
{
    int i;
    int j;

    j = 0;
    i = 0;
    while (i < data->philos)
    {
      if (data->number_of_times_to_eat <= data->number_of_meals[i])
        i++;
      else
        return (1);
    }
    return (0);
}

/* set the stop_simulation to true if a philosopher dies*/
void *monitor1(void *v_data)
{
  t_data *data =  (t_data *)v_data;
  int i = 0;
  printf("monitor thread has been launched\n");
  while (i < data->philos)
    gettimeofday(&(data->hungry_time[i++]), NULL);
  while (1)
  {
    // cheks each philo time to live
    i = 0;
    while (i < data->philos)
    {
      /*pthread_mutex_lock(&(data->stop_simulation_mutex));
      if (data->stop_simulation == true)
        return (NULL);
      pthread_mutex_unlock(&(data->stop_simulation_mutex));*/
      //if (data->stop_simulation == true)
        //return (NULL);
      if (getCurrentTime(&data->hungry_time[i]) >= data->time_to_die)
      {
        pthread_mutex_lock(&(data->stop_simulation_mutex));
        data->stop_simulation = true;
        //printf("--------[%d]ms philospher %d dead detected by the monitor!!!!-------\n", getCurrentTime(&data->hungry_time[i]), i + 1);
        pthread_mutex_unlock(&(data->stop_simulation_mutex));
        return (NULL);
      }
      i++;
    }
  }
  return (NULL);
}

int main(int ac, char **av)
{
  if (!(ac == 5 || ac == 6))
    return (0);
  t_data *data = malloc(sizeof(t_data));
  parse_data(data, av, ac);
  gettimeofday(&(data->currentTime), NULL); // ? 
  //data->start_time = data->currentTime.tv_usec; // ? 
  pthread_mutex_init(&(data->i_mutex), NULL);
  pthread_mutex_init(&(data->stop_simulation_mutex), NULL);
  pthread_mutex_init(&(data->number_of_meals_mutex), NULL);
  pthread_cond_init(&(data->iCond), NULL);
  pthread_create((data->monitor_thread), NULL, &monitor1, (void *)data);
  for (int i = 0 ; i < data->philos; i++)
  {
    pthread_mutex_lock(&(data->i_mutex));
    data->philo_id = i;
    if (i % 2 == 1)
      pthread_create(data->ph_th + i, NULL, &routine1, (void *)data);
    else
      pthread_create(data->ph_th + i, NULL, &routine2, (void *)data);
    while (i == data->philo_id)
      pthread_cond_wait(&(data->iCond), &(data->i_mutex));
    pthread_mutex_unlock(&(data->i_mutex));
  }
  for (int i = 0 ; i < data->philos ; i++)
    pthread_join(data->ph_th[i], NULL);
  pthread_join(*(data->monitor_thread), NULL);
  //pthread_mutex_destroy(&(data->forkMutex));
  pthread_mutex_destroy(&(data->i_mutex));
  pthread_cond_destroy(&(data->iCond));
  return (0);
}

/*int forks(int philo_id, int philos_number)
{
  int left_fork = philo_id;
  int right_fork;
  if (philo_id == philo_number)
    right_fork = 1;
  else
    right_fork = philo_id +
}*/