#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define ITERATIONS 2000000000

int main()
{
	int i;
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Start time

	double result = 0;
	for (i = 0; i < ITERATIONS; i++)
	{
		// printf("%f\n", (i & 1 ? -1.0 : 1.0) * 1 / (2 * i + 1));
		result += (i & 1 ? -1.0 : 1.0) / (2 * i + 1);
	}

	printf("Result for %d: %f\n", ITERATIONS, result * 4);

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // End time

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TOTAL TIME: %d seconds\n", (int)elapsed_time);
}