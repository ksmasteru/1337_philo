#include "philo.h"


int get_philo_args(t_data *data, char **av, int ac)
{
  data->stop_simulation = false;
  data->philos = ft_atoi(av[1]);
  if (data->philos < 0)
    return (1);
  data->time_to_die = ft_atoi(av[2]);
  if (data->time_to_die < 0 || data->time_to_die > INT_MAX)
    return (1);
  data->time_to_eat = ft_atoi(av[3]);
  if (data->time_to_eat < 0 || data->time_to_eat > INT_MAX)
    return (1);
  data->time_to_sleep = ft_atoi(av[4]);
  if (data->time_to_sleep < 0 || data->time_to_sleep > INT_MAX)
    return (1);
  data->time_elapsed = 0;
  if (data->time_elapsed < 0)
    return (1);
  if (ac == 6)
  {
    data->number_of_times_to_eat = ft_atoi(av[5]);
    if (data->number_of_times_to_eat <= 0 || data->number_of_times_to_eat > INT_MAX)
      return (1);
  }
  else
    data->number_of_times_to_eat = -1;
  return (0);
}

int data_mem_alloc(t_data *data)
{
  data->ph_th = (pthread_t *) malloc(sizeof(pthread_t) * data->philos);
  if (!data->ph_th)
    return (1);
  data->hungry_time = (struct timeval *) malloc(sizeof(struct timeval) * data->philos);
  if (!data->hungry_time)
    return (1);
  data->forkMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->philos);
  if (!data->forkMutex)
    return (1);
  data->done_eating_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->philos);
  if (!data->done_eating_mutex)
    return (1);
  data->monitor_thread = (pthread_t *)malloc(sizeof(pthread_t));
  if (!data->monitor_thread)
    return (1);
  data->hungry_time_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_t));
  if (!data->hungry_time_mutex)
    return (1);
  data->done_eating = (int *)malloc(sizeof(int) * data->philos);
  if (!data->done_eating)
    return (1);
  return (0);
}
int parse_data(t_data *data, char **av, int ac)
{
  int i;

  i = -1;
  if (get_philo_args(data, av, ac) != 0)
  {
    printf("error");
    free(data);
    return (1);
  }
  if (data_mem_alloc(data) != 0)
  {
    printf("error");
    free(data);
    return (1);
  }
  while (++i < data->philos)
  {
    pthread_mutex_init(&(data->forkMutex[i]), NULL); 
    data->done_eating[i] = 0;
  }
  return (0);
}

/* 
  returns elapsed time in ms since the initialiaztion of the struct parameter
  returns -1 when the philosopher is done eating.
*/
int getCurrentTime(struct timeval *currentTime)
{
  struct timeval elapsedTime;
  if (!currentTime)
    return (-1);
  gettimeofday(&elapsedTime, NULL);
  return (elapsedTime.tv_sec * 1000 + elapsedTime.tv_usec / 1000) - ((currentTime->tv_sec) * 1000 + (currentTime->tv_usec) / 1000);
}

void fill_philo_data(t_data *data, int *philo_data)
{
  pthread_mutex_lock(&(data->i_mutex));
  philo_data[INDEX] = data->philo_id;
  philo_data[PHILO_ID] = 1 + data->philo_id--; // ??
  philo_data[RIGHT_FORK] = philo_data[PHILO_ID];
  philo_data[TIMES_EATING] = 0;
  if (data->philos == philo_data[PHILO_ID])
    philo_data[LEFT_FORK] = 1;
  else
    philo_data[LEFT_FORK] = philo_data[RIGHT_FORK] + 1;
  pthread_mutex_unlock(&(data->i_mutex));
}
void *routine1(void  *v_data)
{
  int philo_data[5]; //test with 200 philo
  t_data *data;

  data = (t_data *)(v_data);
  fill_philo_data(data, philo_data);
  printf("[%d]ms philo number %d is eating at the start\n", getCurrentTime(&data->currentTime), philo_data[PHILO_ID]);
  ft_usleep(data->time_to_sleep * 1000 * 0.2);
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

void *single_philo(t_data *data)
{
  pthread_mutex_lock(data->forkMutex);
  printf("[%d]ms philosophere 1 has locked left fork\n", getCurrentTime(&data->currentTime));
  ft_usleep(data->time_to_die * 1000);
  pthread_mutex_lock(&data->stop_simulation_mutex);
  data->stop_simulation = true;
  pthread_mutex_unlock(&data->stop_simulation_mutex);
  pthread_mutex_unlock(data->forkMutex);
  printf("[%d]ms philosopher 1 has died x_x\n", getCurrentTime(&data->currentTime));
  return (NULL);
}
void *routine2(void  *v_data)
{
  int philo_data[5]; //test with 200 philo
  t_data *data;

  data = (t_data *)(v_data);
  fill_philo_data(data, philo_data);
  //pthread_cond_signal(&(data->iCond));
  if (data->philos  == 1)
    return (single_philo(data));  
    //printf("philsopher %d is not eating at the start\n", philo_id);
  while (data->number_of_times_to_eat != philo_data[TIMES_EATING])
  {
    if (!sleep_and_think(data, philo_data))
      break;
    if(!take_fork_and_eat(data, philo_data))
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

/* return true if each philos has at least eaten number_of_times_to_Eat
can this be done by each philosopher ?*/
bool check_number_meals(t_data *data)
{
    int i;
    int j;

    j = 0;
    i = 0;
    while (i < data->philos)
    {
      pthread_mutex_lock(data->done_eating_mutex);
      if (data->done_eating[i++] == 0)
      {
        pthread_mutex_unlock(data->done_eating_mutex);
        return (false);
      }
      pthread_mutex_unlock(data->done_eating_mutex);
    }
    return (true);
}

void set_hungry_time(t_data *data)
{
  int i;

  i = 0;
  pthread_mutex_lock(data->hungry_time_mutex);
  while (i < data->philos)
    gettimeofday(&(data->hungry_time[i++]), NULL);
  pthread_mutex_unlock(data->hungry_time_mutex);
}

bool is_done_eating(t_data *data, int index)
{
  pthread_mutex_lock(data->done_eating_mutex);
  if (data->done_eating[index] == 1)
  {
    pthread_mutex_unlock(data->done_eating_mutex);
    return (true);
  }
  pthread_mutex_unlock(data->done_eating_mutex);
  return (false);
}
bool is_still_alive(t_data *data, int index)
{
  pthread_mutex_lock(data->hungry_time_mutex);
  data->time_elapsed = getCurrentTime(&data->hungry_time[index]);
  pthread_mutex_unlock(data->hungry_time_mutex);
  if (data->time_elapsed > data->time_to_die)
  {
    pthread_mutex_lock(&(data->stop_simulation_mutex));
    data->stop_simulation = true;
    printf("--------[%d]ms philospher %d dead detected by the monitor!!!!-------\n", getCurrentTime(&data->hungry_time[index]), index + 1);
    pthread_mutex_unlock(&(data->stop_simulation_mutex));
    return (false);
  }
  return (true);
}

void *monitor1(void *v_data)
{
  t_data *data;
  int i;

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
        if (!is_still_alive(data, i)) /*pthread join to terminate the threads ? 5 800 300 200*/
          return (NULL);
      }
      i++;
    }
  }
  return (NULL);
}
int init_mutexes(t_data *data)
{
  pthread_mutex_init(&(data->i_mutex), NULL);
  pthread_mutex_init(&(data->stop_simulation_mutex), NULL);
  pthread_mutex_init(&(data->number_of_meals_mutex), NULL);
  pthread_mutex_init(data->hungry_time_mutex, NULL);
  pthread_mutex_init(data->done_eating_mutex, NULL);
  return (0);
}

int create_threads(t_data *data)
{
  for (int i = 0 ; i < data->philos; i++)
  {
    pthread_mutex_lock(&(data->i_mutex));
    data->philo_id = i;
    if (i % 2 == 1)
      pthread_create(data->ph_th + i, NULL, &routine1, (void *)data);
    else
      pthread_create(data->ph_th + i, NULL, &routine2, (void *)data);
    pthread_mutex_unlock(&(data->i_mutex));
    usleep(100);
  }
  pthread_create((data->monitor_thread), NULL, &monitor1, (void *)data);
  for (int i = 0 ; i < data->philos ; i++)
    pthread_join(data->ph_th[i], NULL);
  pthread_join(*(data->monitor_thread), NULL);
  return (0);
}

void free_data(t_data *data)
{
  free(data->done_eating);
  free(data->ph_th);
  free(data->monitor_thread);
  free(data->forkMutex);
  free(data->hungry_time_mutex);
  free(data->done_eating_mutex);
  free(data);
}

void destroy_mutexes(t_data *data)
{
  pthread_mutex_destroy(&(data->i_mutex));
  pthread_mutex_destroy(data->hungry_time_mutex);
  pthread_mutex_destroy(&data->stop_simulation_mutex);
  pthread_mutex_destroy(&data->number_of_meals_mutex);
  pthread_mutex_destroy(data->done_eating_mutex);
}
int main(int ac, char **av)
{
  if (!(ac == 5 || ac == 6))
    return (0);
  t_data *data = malloc(sizeof(t_data));
  if (parse_data(data, av, ac) != 0)
    return (0);
  gettimeofday(&(data->currentTime), NULL);
  init_mutexes(data);
  pthread_cond_init(&(data->iCond), NULL);
  create_threads(data);
  pthread_cond_destroy(&(data->iCond));
  free_data(data);
  return (0);
}