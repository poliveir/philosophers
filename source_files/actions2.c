#include "../header_files/philosophers.h"

void	pick_up_fork2(t_phil *ph, char side)
{
	if (side == 'r')
	{
		ph->table->forks[ph->f_right] = 0;
		ph->n_forks++;
		pthread_mutex_lock(&ph->table->print);
		if (!ph->table->death_is_present && ph->table->all_saciated != ph->table->n_phil)
			printf("%ld %d has taken a fork\n", \
		check_time() - ph->table->start_time, ph->pos + 1);
		pthread_mutex_unlock(&ph->table->print);
	}
	else if (side == 'l')
	{
		ph->table->forks[ph->f_left] = 0;
		ph->n_forks++;
		pthread_mutex_lock(&ph->table->print);
		if (!ph->table->death_is_present && ph->table->all_saciated != ph->table->n_phil)
			printf("%ld %d has taken a fork\n", \
		check_time() - ph->table->start_time, ph->pos + 1);
		pthread_mutex_unlock(&ph->table->print);
	}
}

void	pick_up_fork(t_phil *ph)
{
	while (ph->n_forks != 2 && !check_deaths(ph) && !check_saciation(ph))
	{
		if ((ph->pos + 1) % 2 == 0)
		{
			pthread_mutex_lock(&ph->table->fork_locks[ph->f_right]);
			if (ph->table->forks[ph->f_right])
				pick_up_fork2(ph, 'r');
			pthread_mutex_unlock(&ph->table->fork_locks[ph->f_right]);
			pthread_mutex_lock(&ph->table->fork_locks[ph->f_left]);
			if (ph->table->forks[ph->f_left])
				pick_up_fork2(ph, 'l');
			pthread_mutex_unlock(&ph->table->fork_locks[ph->f_left]);
		}
		else
		{
			usleep(100);
			pthread_mutex_lock(&ph->table->fork_locks[ph->f_left]);
			if (ph->table->forks[ph->f_left])
				pick_up_fork2(ph, 'l');
			pthread_mutex_unlock(&ph->table->fork_locks[ph->f_left]);
			pthread_mutex_lock(&ph->table->fork_locks[ph->f_right]);
			if (ph->table->forks[ph->f_right])
				pick_up_fork2(ph, 'r');
			pthread_mutex_unlock(&ph->table->fork_locks[ph->f_right]);
		}

	}
}

void	start_eating(t_phil *ph)
{
	if (!check_deaths(ph) && !check_saciation(ph))
	{
		ph->last_meal_time = check_time();
		ph->eating = 1;
        //ph->meals++;
		pthread_mutex_lock(&ph->table->print);
		if (!ph->table->death_is_present && ph->table->all_saciated != ph->table->n_phil)
			printf("%ld %d is eating\n", \
			check_time() - ph->table->start_time, ph->pos + 1);
		pthread_mutex_unlock(&ph->table->print);
		if (ph->meals_left != -1)
			ph->meals_left--;
		if (ph->meals_left == 0)
		{
			ph->meals_left = -1;
			pthread_mutex_lock(&ph->table->saciation);
			ph->table->all_saciated++;
			pthread_mutex_unlock(&ph->table->saciation);
		}
	}
}