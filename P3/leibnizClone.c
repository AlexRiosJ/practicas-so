#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#define SIZESTACK (1024 * 64)
#define ITERATIONS 2000000000
#define NTHREADS 4

double result = 0;
double threadResults[NTHREADS];

int tfunc(void *args)
{
    int nthread = *((int *)args);
    int i;
    int inicio = nthread * (ITERATIONS / NTHREADS);
    int fin = (nthread + 1) * (ITERATIONS / NTHREADS);

    for (i = inicio; i < fin; i++)
	{
		threadResults[nthread] += (pow(-1, i) / (2 * i + 1));
	}
}

int main()
{
    int i;
    int numberArray[NTHREADS];
    pid_t tid[NTHREADS];

    long long start_ts;
    long long stop_ts;
    long long elapsed_time;
    long lElapsedTime;
    struct timeval ts;

    char *stack;
    char *stackhead;

    stack = (char *) malloc(SIZESTACK * NTHREADS);

    int status;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; // Start time

    // Create Threads
    for (i = 0; i < NTHREADS; i++)
    {
        stackhead = stack + (SIZESTACK * (i + 1));
        numberArray[i] = i;
        tid[i] = clone(tfunc, stackhead, CLONE_VM | SIGCHLD, (void *) &numberArray[i]);
    }

    for (i = 0; i < NTHREADS; i++)
        wait(&status);

    free(stack);

    for (i = 0; i < NTHREADS; i++)
        result += threadResults[i];

    printf("Result for %d: %f\n", ITERATIONS, result);

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; // End time

    elapsed_time = stop_ts - start_ts;
    printf("------------------------------\n");
    printf("TOTAL TIME: %d seconds\n", (int) elapsed_time);
}