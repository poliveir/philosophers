#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>

typedef struct s_table
{
	long			start_time;
	int				n_phil;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_to_eat;
	int				*forks;
	pthread_mutex_t	*fork_locks;
	int				death_is_present;
	int				all_saciated;
	pthread_mutex_t	saciation;
	pthread_mutex_t	death;
	pthread_mutex_t print;
	pthread_t		*th;
}				t_table;

typedef struct s_phil
{
	int		pos;
	int		meals_left;
	long	last_meal_time;
	long	start_sleeping_time;
	int		f_left;
	int		f_right;
	int		n_forks;
	int		eating;
	int		sleeping;
	int		thinking;
	t_table	*table;
}				t_phil;

/*Philosophers:*/

int		error_handling(int error, t_phil *ph);

int		free_everything(t_phil *ph);

long	check_time(void);

void	pick_up_fork(t_phil *ph);

void	pick_up_fork2(t_phil *ph, char side);

void	start_eating(t_phil *ph);

void	stop_eating(t_phil *ph);

void	stop_sleeping(t_phil *ph);

void	start_sleeping(t_phil *ph);

int		check_deaths(t_phil *ph);

int		check_saciation(t_phil *ph);

/*Libft:*/

int		ft_atoi(const char *str);

#endif