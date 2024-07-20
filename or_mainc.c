/* #include "philo.h"

void parse_data(t_data *data, char **av)
{
  int i;

  i = -1;
  data->philos = atoi(av[1]);
  data->time_to_die = atoi(av[2]);
  data->time_to_eat = atoi(av[3]);
  data->time_to_sleep = atoi(av[4]);
  data->available_forks = data->philos;
  data->ph_th = (pthread_t *) malloc(sizeof(pthread_t) * data->philos);
  data->hungry_time = (struct timeval *) malloc(sizeof(struct timeval) * data->philos);
  data->forkMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->philos);
  data->forkCond = (pthread_cond_t *) malloc(sizeof(pthread_cond_t) * data->philos);
  while (++i < data->philos)
  {
    pthread_mutex_init(&(data->forkMutex[i]), NULL); 
    pthread_cond_init(&(data->forkCond[i]), &(data->forkMutex[i]));
  }
}

int getCurrentTime(struct timeval *currentTime)
{
  struct timeval elapsedTime;
  gettimeofday(&elapsedTime, NULL);
  return (elapsedTime.tv_sec * 1000 + elapsedTime.tv_usec / 1000) - ((currentTime->tv_sec) * 1000 + (currentTime->tv_usec) / 1000);
  //printf("time passed is %d\n", time_passed);
  //return (time_passed);
}

void *routine1(void *v_data)
{
  // each fork protected 
  //time_to_die (in milliseconds): 
  //If a philosopher didn’t start eating time_to_die
  //milliseconds since the beginning of their last meal
  //or the beginning of the simulation, they die.
  t_data *data = (t_data *)v_data;
  pthread_mutex_lock(&(data->i_mutex));
  int index =  data->philo_id--;
  pthread_mutex_unlock(&(data->i_mutex));
  pthread_cond_signal(&(data->iCond));
  gettimeofday(&(data->hungry_time[index]), NULL);
  
  while (getCurrentTime(&(data->hungry_time[index])) <= data->time_to_die)
  {
    // 
    //printf("philo id %d has started routine1\n", index);
    pthread_mutex_lock(data->forkMutex + index - 1);
    data->available_forks -= 1;
    pthread_mutex_lock(data->forkMutex + index); // SIGEV
    data->available_forks -= 1;
    philo_take_fork(data, index);
    philo_eat(data, index);
    data->available_forks += 1;
    pthread_mutex_unlock(data->forkMutex + index - 1);
    // should notify thinking pihlosoers that the for has
    pthread_mutex_signal(data->forkCond + index - 1);
    data->available_forks += 1;
    pthread_mutex_unlock(data->forkMutex + index);
    pthread_mutex_signal(data->forkCond + index);
    philo_sleep(data, index);
    philo_think(data, index); // keeps thinking until he get a fork;
  // all data should be in a mutex;
  //pthread_mutex_unlock (&(data->forkMutex));
  }

  printf("philosopher %d died x_x\n", index);
  return (NULL);
}

void *routine2(void *v_data)
{
  t_data *data = (t_data *)v_data;
  pthread_mutex_lock(&(data->i_mutex));
  int index =  data->philo_id--;
  pthread_mutex_unlock(&(data->i_mutex));
  pthread_cond_signal(&(data->iCond));
  gettimeofday(&(data->hungry_time[index]), NULL);
  while (getCurrentTime(&(data->hungry_time[index])) <= data->time_to_die)
  {
    // RIGHT FOrk is index
    // left fork is imdex - 1;
    // left and right differ from routine 1 routine 2 ?
    philo_sleep(data, index); // prevent lock before 1
    philo_think(data, index); // keeps fdthinking until he gets a fork
    pthread_mutex_lock(data->forkMutex + index - 1);// try lock ?
    while (data->available_forks == 0)
    {
      pthread_cond_wait(data->forkCond + index - 1, data->forkMutex + index- 1);
    }
    data->available_forks -= 1;
    pthread_mutex_lock(data->forkMutex + index);
    while (data->available_forks == 0) /*takes a random fork? */
    /*{
      pthread_cond_wait(data->forkCond + index, data->forkMutex + index);
    }
    data->available_forks -= 1;
    philo_take_fork(data, index);
    philo_eat(data, index);
    //done eatin put down the fork.
    data->available_forks += 2;
    // conditon variable == availabe forks;
    pthread_mutex_unlock(data->forkMutex + index - 1);
    pthread_mutex_unlock(data->forkMutex + index);
  // all data should be in a mutex;
  //pthread_mutex_unlock (&(data->forkMutex));
  }
  printf("philosopher %d died x_x\n", index);
  return (NULL);
}

int main(int ac, char **av)
{
  if (ac < 5)
    return (0);
  t_data *data = malloc(sizeof(t_data));
  parse_data(data, av);
  gettimeofday(&(data->currentTime), NULL); // ? 
  //data->start_time = data->currentTime.tv_usec; // ? 
  pthread_mutex_init(&(data->i_mutex), NULL);
  pthread_cond_init(&(data->iCond), NULL);
  for (int i = 0 ; i < data->philos ; i++)
  {
    pthread_mutex_lock(&(data->i_mutex));
    // RIGHT FOrk is index + 1
    // left fork is imdex;
    data->philo_id = i;
    // wait for a signal that a thread received this varaible
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
  //pthread_mutex_destroy(&(data->forkMutex));
  pthread_mutex_destroy(&(data->i_mutex));
  pthread_cond_destroy(&(data->iCond));
  return (0);
}*/