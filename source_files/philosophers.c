#include "../header_files/philosophers.h"

static t_phil	*set_philosophers(t_table *table)
{
	t_phil	*ph;
	int		i;

	ph = malloc(sizeof(t_phil) * table->n_phil);
	i = 0;
	while (i < table->n_phil)
	{
		ph[i].pos = i;
		ph[i].meals_left = table->times_to_eat;
		ph[i].last_meal_time = table->start_time;
		ph[i].table = table;
		ph[i].f_left = ph[i].pos;
		ph[i].n_forks = 0;
		ph[i].eating = 0;
		ph[i].sleeping = 0;
		ph[i].thinking = 0;
		ph[i].test = 0;
		if (i == table->n_phil - 1)
			ph[i].f_right = 0;
		else
			ph[i].f_right = ph[i].pos + 1;
		i++;
	}
	return (ph);
}

static pthread_t	*set_the_table(t_table *table, int argc, char **argv)
{
	int	i;

	table->n_phil = ft_atoi(argv[1]);
	table->start_time = check_time();
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->times_to_eat = -1;
	table->death_is_present = 0;
	table->all_saciated = 0;
	table->forks = malloc(sizeof(int) * table->n_phil);
	table->fork_locks = malloc(sizeof(pthread_mutex_t) * table->n_phil);
	pthread_mutex_init(&table->mutex, NULL);
	i = 0;
	while (i < table->n_phil)
	{
		pthread_mutex_init(&table->fork_locks[i], NULL);
		table->forks[i++] = 1;
	}
	if (argc == 6)
		table->times_to_eat = ft_atoi(argv[5]);
	return (malloc(sizeof(pthread_t) * table->n_phil));
}

static void	*routine(void *arg)
{
	t_phil	*ph;
	t_table	*table;

	ph = (t_phil *)arg;
	table = ph->table;
	while (!check_deaths(ph) && !check_saciation(ph))
	{
		if (!ph->sleeping && !ph->eating)
		{
			pick_up_fork(ph);
			start_eating(ph);
		}
		if (ph->eating)
		{
			stop_eating(ph);
			if (!ph->eating)
				start_sleeping(ph);
		}
		if (ph->sleeping)
			stop_sleeping(ph);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_table			table;
	t_phil			*ph;
	int				i;

	ph = NULL;
	if (argc < 5 || argc > 6)
		return (error_handling(0, ph));
	table.th = set_the_table(&table, argc, argv);
	ph = set_philosophers(&table);
	i = 0;
	while (i < table.n_phil)
	{
		if (pthread_create(&table.th[i], NULL, &routine, &ph[i]))
			return (error_handling(1, ph));
		i++;
	}
	i = 0;
	while (i < table.n_phil)
	{
		if (pthread_join(table.th[i], NULL))
			return (error_handling(1, ph));
		i++;
	}
	return (free_everything(ph));
}
