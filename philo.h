#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>

#define PHILO_ID 0
#define INDEX 1
#define LEFT_FORK  2
#define RIGHT_FORK 3
#define TIMES_EATING 4

typedef struct s_data{
  bool stop_simulation;
  long philos;
  long time_to_die;
  long time_to_eat;
  long philo_id;
  long time_to_sleep;
  long time_elapsed;
  long number_of_times_to_eat;
  int *number_of_meals;
  int *done_eating;

  struct timeval currentTime;
  struct timeval *hungry_time;
  pthread_t *ph_th;
  pthread_t *monitor_thread;
  pthread_mutex_t number_of_meals_mutex;
  pthread_mutex_t *forkMutex;
  pthread_mutex_t *hungry_time_mutex;
  pthread_mutex_t *done_eating_mutex;
  pthread_mutex_t stop_simulation_mutex;
  pthread_cond_t *forkCond;
  pthread_mutex_t i_mutex;
  pthread_cond_t iCond;
}t_data;
int ft_usleep(double sec);
int philo_take_fork(t_data *data, int index, int left_fork, int right_fork);
int philo_eat(t_data *data, int index);
int philo_think(t_data *data, int index);
int philo_sleep(t_data *data, int index);
bool sleep_and_think(t_data *data, int *philo_data);
bool take_fork_and_eat(t_data *data, int *philo_data);
int getCurrentTime(struct timeval *currentTime);
int	atoi_get_sign(char **str);
long	ft_atoi(char *str);
#endif