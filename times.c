#include "collider.h"

long long	get_time(void)
{
	static struct timeval	tv;
	static struct timezone	tz;
	long long				res;

	gettimeofday(&tv, &tz);
	res = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (res);
}

int	get_other_time()
{
	static long long		start = 0;
	int						res;
	if (start == 0)
	{
		start = get_time();
		return (0);
	}
	else
	{
		res = (int)(get_time() - start);
		return (res);
	}
}