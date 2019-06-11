#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define ITERATIONS 2000000000
#define NTHREADS 4

// Compile with: gcc -o leibnizPosix leibnizPosix.c -lm -lpthread

double result = 0;
double threadResults[NTHREADS];

void *tfunc(void *args)
{
	int nthread = *((int *)args);
	int i;
	int inicio = nthread * (ITERATIONS / NTHREADS);
	int fin = (nthread + 1) * (ITERATIONS / NTHREADS);
	
	for (i = inicio; i < fin; i++)
	{
		threadResults[nthread] += (i & 1 ? -1.0 : 1.0) / (2 * i + 1);
	}
}

int main()
{
	int i;
	int numberArray[NTHREADS];
	pthread_t tid[NTHREADS];

	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Start time

	// Crear los hilos
	for (i = 0; i < NTHREADS; i++)
	{
		numberArray[i] = i;
		pthread_create(&tid[i], NULL, tfunc, (void *) &numberArray[i]);
	}

	for (i = 0; i < NTHREADS; i++)
		pthread_join(tid[i], NULL);

	for (i = 0; i < NTHREADS; i++)
		result += threadResults[i];

	printf("Result for %d: %f\n", ITERATIONS, result);

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // End time

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TOTAL TIME: %d seconds\n", (int) elapsed_time);
}