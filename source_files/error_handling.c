#include "../header_files/philosophers.h"

#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define NORMAL "\033[0m"

int	error_handling(int error, t_phil *ph)
{
	printf("%s[ERROR]%s ", RED, NORMAL);
	if (error == 0)
	{
		printf("Wrong number of arguments\n");
		printf("Please execute the program as follows:\n");
		printf("./philsophers number_of_philosophers time_to_die");
		printf(" time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat] -> (optional)\n");
		return (0);
	}
	else if (error == 1)
		printf("Unable to create or joining a philosopher(thread)\n");
	return (free_everything(ph));
}
