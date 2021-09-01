# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

int main()
{
	struct timeval timeval;
	int	time;

	gettimeofday(&timeval, NULL);
	time = timeval.tv_sec;
	sleep(5);
	gettimeofday(&timeval, NULL);
	printf("TIME -> %d\n", timeval.tv_sec - time);
}