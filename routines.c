#include "philo.h"
/*stop_simulation == true ? return -1 does unlock mutexes: return 0 and mutex have to unlocked*/

int philo_take_fork(t_data *data, int index, int left_fork, int right_fork)
{
    //printf("[%d]ms philo number %d is trying to lock left fork number [%d]\n",getCurrentTime(&data->currentTime), index + 1 , left_fork);
    pthread_mutex_lock(&data->stop_simulation_mutex);
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        return (-1);
    }
    pthread_mutex_unlock(&data->stop_simulation_mutex);
    pthread_mutex_lock(data->forkMutex + left_fork - 1); // fork index + 1;
    pthread_mutex_lock(&data->stop_simulation_mutex);
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(data->forkMutex + left_fork - 1);
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        return (-1);
    }
    printf("[%d]ms philo number %d locked left fork number[%d] succesfully\n",getCurrentTime(&data->currentTime), index + 1 , left_fork);
    pthread_mutex_unlock(&data->stop_simulation_mutex);
    pthread_mutex_lock(data->forkMutex + right_fork - 1); // SIGEV
    pthread_mutex_lock(&data->stop_simulation_mutex);
    if (data->stop_simulation == true)
    {
        pthread_mutex_unlock(data->forkMutex + left_fork - 1);
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        pthread_mutex_unlock(data->forkMutex + right_fork - 1);
        return (-1);
    }
    pthread_mutex_unlock(&data->stop_simulation_mutex);
    printf("[%d]ms philo number %d locked right fork number [%d] succesfully\n",getCurrentTime(&data->currentTime), index + 1 , right_fork);
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
    //pthread_mutex_lock(&(data->number_of_meals_mutex));
    //data->number_of_meals[index]++;
    //pthread_mutex_unlock(&(data->number_of_meals_mutex));
    //printf("philosopher number %d has eaten %d times\n", index + 1, data->number_of_meals[index]);
    usleep(data->time_to_eat * 1000);
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
    //thinks until he gets a fork. condvariable;
    // wait for a signal == cond_variable
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
        usleep(1000 * (data->time_to_die - getCurrentTime(&data->hungry_time[index])));
        pthread_mutex_lock(&data->stop_simulation_mutex); // let know the monitor that a philo died.
        data->stop_simulation = true;
        pthread_mutex_unlock(&data->stop_simulation_mutex);
        printf("--------philosopher %d died x_x while sleeping----------\n", index + 1); // change to standar philo died
        return (-1);
    }
    else
        usleep(data->time_to_sleep * 1000);
    return (0);
}