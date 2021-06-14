#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>


/*
long int	gettimediff(struct timeval tzero)
{
	struct timeval	tv;
	struct timezone	tz;
	long int		diff;

	gettimeofday(&tzero, &tz);
	diff = (tv.tv_sec - tzero.tv_sec) * 1000 + 
}
*/

int main ()
{


	struct timeval	tv;
	struct timeval	tzero;
	struct timezone tz;
	
	gettimeofday(&tzero, &tz);
	while (1)
	{
		gettimeofday(&tv, &tz);
		printf("%ld%03.3d\n", (tv.tv_sec - tzero.tv_sec) % 100, tv.tv_usec / 1000);
	}

}
