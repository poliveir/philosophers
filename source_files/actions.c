#include "../header_files/philosophers.h"

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
		pthread_mutex_lock(&ph->table->print);
		if (!ph->table->death_is_present && ph->table->all_saciated != ph->table->n_phil)
			printf("%ld %d is sleeping\n", \
			check_time() - ph->table->start_time, ph->pos + 1);
		pthread_mutex_unlock(&ph->table->print);
	}
}

void	stop_sleeping(t_phil *ph)
{
	if (!check_deaths(ph) && !check_saciation(ph))
	{
		if (check_time() - ph->start_sleeping_time >= ph->table->time_to_sleep)
		{
			ph->sleeping = 0;
			pthread_mutex_lock(&ph->table->print);
			if (!ph->table->death_is_present && ph->table->all_saciated != ph->table->n_phil)
				printf("%ld %d is thinking\n", \
			check_time() - ph->table->start_time, ph->pos + 1);
			pthread_mutex_unlock(&ph->table->print);
		}
	}
}

int	check_saciation(t_phil *ph)
{
	pthread_mutex_lock(&ph->table->saciation);
	if (ph->table->all_saciated == ph->table->n_phil)
	{
		pthread_mutex_unlock(&ph->table->saciation);
		return (1);
	}
	pthread_mutex_unlock(&ph->table->saciation);
	return (0);
}

int	check_deaths(t_phil *ph)
{
	long	current_time;
	long	fasting_time;

	pthread_mutex_lock(&ph->table->death);
	if (ph->table->death_is_present)
	{
		pthread_mutex_unlock(&ph->table->death);
		return (1);
	}
	fasting_time = check_time() - ph->last_meal_time;
	current_time = check_time() - ph->table->start_time;
	if (fasting_time >= ph->table->time_to_die)
	{
		if (!ph->table->death_is_present && ph->table->all_saciated != ph->table->n_phil)
		{
			ph->table->death_is_present = 1;
			pthread_mutex_lock(&ph->table->print);
			printf("%li %d died\n", check_time() - ph->table->start_time, ph->pos + 1);
			pthread_mutex_unlock(&ph->table->print);
		}
		pthread_mutex_unlock(&ph->table->death);
		return (1);
	}
	pthread_mutex_unlock(&ph->table->death);
	return (0);
}