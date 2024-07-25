/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:44:24 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/25 19:44:59 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

# define PHILO_ID 0
# define INDEX 1
# define LEFT_FORK 2
# define RIGHT_FORK 3
# define TIMES_EATING 4

typedef struct s_stack
{
	int				data;
	struct s_stack	*next;
}					t_stack;

typedef struct s_data
{
	bool			stop_simulation;
	long			philos;
	long			time_to_die;
	long			time_to_eat;
	long			philo_id;
	long			time_to_sleep;
	long			time_elapsed;
	long			number_of_times_to_eat;
	int				*number_of_meals;
	int				*done_eating;
	char			**num_str;

	struct timeval	currenttime;
	struct timeval	*hungry_time;
	pthread_t		*ph_th;
	t_stack			*a_head;
	pthread_t		*monitor_thread;
	pthread_mutex_t	number_of_meals_mutex;
	pthread_mutex_t	*forkmutex;
	pthread_mutex_t	*hungry_time_mutex;
	pthread_mutex_t	*done_eating_mutex;
	pthread_mutex_t	get_current_time_mutex;
	pthread_mutex_t	stop_simulation_mutex;
	pthread_cond_t	*forkcond;
	pthread_mutex_t	i_mutex;
}					t_data;

void				*monitor1(void *v_data);
void				*routine1(void *v_data);
void				*single_philo(t_data *data);
void				*routine2(void *v_data);
int					ft_usleep(double sec);
int					philo_take_fork(t_data *data, int index, int left_fork,
						int right_fork);
int					philo_eat(t_data *data, int index);
int					philo_think(t_data *data, int index);
int					philo_sleep(t_data *data, int index);
bool				sleep_and_think(t_data *data, int *philo_data);
bool				take_fork_and_eat(t_data *data, int *philo_data);
int					getcurrenttime(struct timeval *currenttime);
int					atoi_get_sign(char **str);
long				ft_atoi(char *str);
bool				check_number_meals(t_data *data);
void				set_hungry_time(t_data *data);
bool				is_done_eating(t_data *data, int index);
int					is_still_alive(t_data *data, int index);
void				fill_philo_data(t_data *data, int *philo_data);
int					parse_data(t_data *data);
int					get_philo_args(t_data *data, char **av, int ac);
int					data_mem_alloc(t_data *data);
int					print_message(t_data *data, int index, int msg_index);
t_stack				*ft_parse(int ac, char **av);
int					das_parsing(t_data *data);
int					count_words(char *cmd, char sep);
char				*make_words(char *str, char sep);
char				**ft_split(char *cmd, char sep);
void				free_splited(char ***res, int i);
void				free_list(t_stack *head);
void				*routine4(void *v_data);
void				*routine3(void *v_data);
void				*one_philo(t_data *data, int left_fork);
int					init_mutexes(t_data *data);
void				free_2d_str(char **str);
t_stack				*ft_lst_new(int data);
t_stack				*ft_lstadd_back(t_stack *old, int data);
int					stack_len(t_stack *head);
#endif
