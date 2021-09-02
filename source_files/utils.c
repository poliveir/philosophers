#include "../header_files/philosophers.h"

int	free_everything(t_phil *ph)
{
	int	i;

	free(ph->table->forks);
	ph->table->forks = NULL;
	pthread_mutex_destroy(&ph->table->saciation);
	pthread_mutex_destroy(&ph->table->death);
	pthread_mutex_destroy(&ph->table->print);
	i = 0;
	while (i < ph->table->n_phil)
		pthread_mutex_destroy(&ph->table->fork_locks[i++]);
	free(ph->table->fork_locks);
	ph->table->fork_locks = NULL;
	free(ph->table->th);
	ph->table->th = NULL;
	free(ph);
	ph = NULL;
	return (0);
}

long	check_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

static int	ft_iswspace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int	sign;
	int	nbr;

	while (ft_iswspace(*str))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	if (ft_isdigit(*str))
	{
		nbr = 0;
		while (ft_isdigit(*str))
			nbr = (nbr * 10) + (*str++ - '0');
		return (nbr * sign);
	}
	return (0);
}