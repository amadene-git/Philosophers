#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include<string.h>

#define INITIAL_STOCK   20
#define NB_CLIENTS      5

int		ft_atoi(char *str)
{
	long int	nbr;
	int			a;

	nbr = 0;
	a = 1;
	while (*str == ' ')
		str++;
	if (*str == '-' && str++)
		a = -1;
	while (*str >= '0' && *str <= '9')
	{
		nbr += (*str - '0');
		nbr *= 10;
		str++;
	}
	nbr /= 10;
	return (nbr * a);
}

int	getrandom()
{
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);
	return ((int)(tv.tv_usec % 10));
}

typedef struct	s_stock
{
	int 			stock;
	pthread_t		thread_store;
	pthread_t		thread_client[NB_CLIENTS];
	pthread_mutex_t	mutex_stock;
}				t_stock;

static	t_stock	store =
{
	.stock = INITIAL_STOCK,
	.mutex_stock = PTHREAD_MUTEX_INITIALIZER,
};


void	*fct_store(void *p_data)
{
	while (1)
	{
	//	printf("store wat unlock\n");
		pthread_mutex_lock(&store.mutex_stock);
		if (store.stock <= 0)
		{
			store.stock = INITIAL_STOCK;
			printf("Le magasin remplit son stock +%d\n", store.stock);
		}
		pthread_mutex_unlock(&store.mutex_stock);
	}
	return (NULL);
}

void	pusleep(int usec)
{
	int i = -1;
	while (++i < usec)
		usleep(1);
}

void	*fct_client(void *p_data)
{
	int	nb;
	int	val;
	int	i;

	nb = (int)p_data;
	i = 0;
	while (1)
	{
	
		val = getrandom();
		pusleep(10000);
		if (store.stock - val >= 0)
		{
			pthread_mutex_lock(&store.mutex_stock);
			store.stock -= val;
			printf("Le client %d prend %d et il reste %d en stock\n", nb, val, store.stock);	
			pthread_mutex_unlock(&store.mutex_stock);
		}
	}
	return (NULL);
}

int main (int ac, char **av)
{
	int ret;
	int i;

	printf("Creation thread store...\n");
	ret = pthread_create(&store.thread_store, NULL, fct_store, NULL);

	if (!ret)
	{
		printf("Creation thread clients...\n");
		for (i = 0; i < NB_CLIENTS; i++)
		{
			printf("client %d cree\n", i);	
			ret = pthread_create(&store.thread_client[i], NULL, fct_client, (void *) i);
			if (ret)
				dprintf(2, "%s\n", strerror(ret));
		}
	
	}
	else
		dprintf(2, "%s\n", strerror(ret));
	i = 0;
	for (i = 0; i < NB_CLIENTS; i++)
		pthread_join(store.thread_client[i], NULL);
	printf("fini !\n");
	pthread_join(store.thread_store, NULL);
	return (0);
}
