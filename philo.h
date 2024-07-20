#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct s_data{
  bool stop_simulation;
  int philos;
  int time_to_die;
  int time_to_eat;
  int philo_id;
  int time_to_sleep;
  int available_forks;
  int number_of_times_to_eat;
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

int philo_take_fork(t_data *data, int index, int left_fork, int right_fork);
int philo_eat(t_data *data, int index);
int philo_think(t_data *data, int index);
int philo_sleep(t_data *data, int index);
int getCurrentTime(struct timeval *currentTime);
#endif