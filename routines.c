#include "philo.h"

int ft_usleep(double sec)
{
    int i;

    i = 0;
    while (i++ < 10)
        usleep(sec/10);
    return (0);
}
bool sleep_and_think(t_data *data, int *philo_data)
{
    if (philo_sleep(data, philo_data[INDEX]) < 0)
      return (false);
    if (philo_think(data, philo_data[INDEX]) < 0)
      return (false);
    return (true);
}

bool take_fork_and_eat(t_data *data, int *philo_data)
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
int philo_take_fork(t_data *data, int index, int left_fork, int right_fork)
{
    pthread_mutex_lock(&data->stop_simulation_mutex);
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        return (-1);
    }
    pthread_mutex_unlock(&data->stop_simulation_mutex);
    pthread_mutex_lock(data->forkMutex + left_fork - 1);
    /*pthread_mutex_lock(&data->stop_simulation_mutex); //norminette
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(data->forkMutex + left_fork - 1);
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        return (-1);
    }*/
    printf("[%d]ms philo number %d has taken a fork\n",getCurrentTime(&data->currentTime), index + 1);
    //pthread_mutex_unlock(&data->stop_simulation_mutex);
    pthread_mutex_lock(data->forkMutex + right_fork - 1);
    pthread_mutex_lock(&data->stop_simulation_mutex);
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(data->forkMutex + left_fork - 1);
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        pthread_mutex_unlock(data->forkMutex + right_fork - 1);
        return (-1);
    }
    pthread_mutex_unlock(&data->stop_simulation_mutex);
    printf("[%d]ms philo number %d has taken a fork\n",getCurrentTime(&data->currentTime), index + 1);
    return (0);
}

/*return ds*/
int philo_eat(t_data *data, int index)
{
    // check before eatimg 
    pthread_mutex_lock(&data->stop_simulation_mutex);
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        return (-1);
    }
    pthread_mutex_unlock(&data->stop_simulation_mutex);
    pthread_mutex_lock(data->hungry_time_mutex);
    gettimeofday(&(data->hungry_time[index]), NULL);/* time reset !! data race*/
    pthread_mutex_unlock(data->hungry_time_mutex);
    pthread_mutex_lock(data->hungry_time_mutex);
    printf("[%d]ms philosopher number %d is eating and his hungry time is %d\n",getCurrentTime(&(data->currentTime)), index + 1, getCurrentTime(&(data->hungry_time[index])));
    pthread_mutex_unlock(data->hungry_time_mutex);
    ft_usleep(data->time_to_eat * 1000);
    return (0);
}

int philo_think(t_data *data, int index)
{
    pthread_mutex_lock(&data->stop_simulation_mutex);
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        return (-1);
    }
    pthread_mutex_unlock(&data->stop_simulation_mutex);   
    printf("[%d]ms philosopher number %d is thinking\n",getCurrentTime(&(data->currentTime)), index + 1);
    return (0);
}

int philo_sleep(t_data *data, int index)
{
    pthread_mutex_lock(&data->stop_simulation_mutex);
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        return (-1);
    }
    pthread_mutex_unlock(&data->stop_simulation_mutex);   
    printf("[%d]ms philosopher number %d is sleeping\n",getCurrentTime(&(data->currentTime)), index + 1);
    if (data->time_to_sleep >= (data->time_to_die - getCurrentTime(&(data->hungry_time[index])))) /* can this be handled by the monitor ? idts*/
    {
        ft_usleep(1000 * (data->time_to_die - getCurrentTime(&data->hungry_time[index])));
        pthread_mutex_lock(&data->stop_simulation_mutex);
        data->stop_simulation = true;
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        printf("--------philosopher %d died x_x while sleeping----------\n", index + 1); // change to standar philo died
        return (-1);
    }
    else
        ft_usleep(data->time_to_sleep * 1000);
    return (0);
}
