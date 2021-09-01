#include "../header_files/philosophers.h"

void	pick_up_fork2(t_phil *ph, char side)
{
	if (side == 'r')
	{
		printf("%ld %d has taken a fork\n", \
		check_time() - ph->table->start_time, ph->pos + 1);
		ph->table->forks[ph->f_right] = 0;
		ph->n_forks++;
	}
	else if (side == 'l')
	{
		printf("%ld %d has taken a fork\n", \
		check_time() - ph->table->start_time, ph->pos + 1);
		ph->table->forks[ph->f_left] = 0;
		ph->n_forks++;
	}
}

void	pick_up_fork(t_phil *ph)
{
	while (ph->n_forks != 2 && !check_deaths(ph) && !check_saciation(ph))
	{
		if ((ph->pos + 1) % 2 > 0)
		{
			pthread_mutex_lock(&ph->table->mutex);
			if (ph->table->forks[ph->f_right])
				pick_up_fork2(ph, 'r');
			if (ph->table->forks[ph->f_left])
				pick_up_fork2(ph, 'l');
			pthread_mutex_unlock(&ph->table->mutex);
		}
		else
		{
			usleep(100);
			pthread_mutex_lock(&ph->table->mutex);
			if (ph->table->forks[ph->f_left])
				pick_up_fork2(ph, 'l');
			if (ph->table->forks[ph->f_right])
				pick_up_fork2(ph, 'r');
			pthread_mutex_unlock(&ph->table->mutex);
		}
	}
}

void	start_eating(t_phil *ph)
{
	if (!check_deaths(ph) && !check_saciation(ph))
	{
		printf("%ld %d is eating\n", \
		check_time() - ph->table->start_time, ph->pos + 1);
		ph->test++;
		ph->last_meal_time = check_time();
		ph->eating = 1;
		if (ph->meals_left != -1)
			ph->meals_left--;
		if (ph->meals_left == 0)
		{
			pthread_mutex_lock(&ph->table->mutex);
			ph->table->all_saciated++;
			ph->meals_left = -1;
			pthread_mutex_unlock(&ph->table->mutex);
		}
	}
}

void	stop_eating(t_phil *ph)
{
	if (!check_deaths(ph) && !check_saciation(ph))
	{
		if (check_time() - ph->last_meal_time >= ph->table->time_to_eat)
		{
			ph->eating = 0;
			ph->n_forks = 0;
			ph->table->forks[ph->f_left] = 1;
			ph->table->forks[ph->f_right] = 1;
		}
	}
}

void	start_sleeping(t_phil *ph)
{
	if (!check_deaths(ph) && !check_saciation(ph))
	{
		ph->start_sleeping_time = check_time();
		ph->sleeping = 1;
		printf("%ld %d is sleeping\n", \
		ph->start_sleeping_time - ph->table->start_time, ph->pos + 1);
	}
}

void	stop_sleeping(t_phil *ph)
{
	if (!check_deaths(ph) && !check_saciation(ph))
	{
		if (check_time() - ph->start_sleeping_time >= ph->table->time_to_sleep)
		{
			printf("%ld %d is thinking\n", \
			check_time() - ph->table->start_time, ph->pos + 1);
			ph->sleeping = 0;
		}
	}
}

int	check_saciation(t_phil *ph)
{
	pthread_mutex_lock(&ph->table->mutex);
	if (ph->table->all_saciated == ph->table->n_phil)
	{
		pthread_mutex_unlock(&ph->table->mutex);
		return (1);
	}
	pthread_mutex_unlock(&ph->table->mutex);
	return (0);
}

int	check_deaths(t_phil *ph)
{
	long	current_time;
	long	fasting_time;

	pthread_mutex_lock(&ph->table->mutex);
	if (ph->table->death_is_present)
	{
		pthread_mutex_unlock(&ph->table->mutex);
		return (1);
	}
	fasting_time = check_time() - ph->last_meal_time;
	current_time = check_time() - ph->table->start_time;
	if (fasting_time >= ph->table->time_to_die)
	{
		if (!ph->table->death_is_present)
			printf("%li %d died\n", current_time, ph->pos + 1);
		ph->table->death_is_present = 1;
		pthread_mutex_unlock(&ph->table->mutex);
		return (1);
	}
	pthread_mutex_unlock(&ph->table->mutex);
	return (0);
}