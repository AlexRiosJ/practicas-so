#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>

#define ITERATIONS 2000000000
#define NTHREADS 4

double result = 0;

void tfunc(void *args)
{
    int nthread = *((int*) args);
    int i;
    int inicio = nthread * (ITERATIONS / NTHREADS);
    int fin = (nthread + 1) * (ITERATIONS / NTHREADS);
    double threadResult = 0.0;

    for (i = inicio; i < fin; i++)
    {
        threadResult += (i & 1 ? -1.0 : 1.0) / (2.0 * i + 1.0);
    }

    char filename[8];
    sprintf(filename, "result%d", nthread);
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "%lf\n", threadResult);
	fclose(fp);
	exit(nthread); // Sends shutdown state to init
}

int main()
{
    int i;
    int parent = getpid();
    int pid;
    int status;
    int numberArray[NTHREADS];
    double threadResults[NTHREADS];
    char line[50];

    long long start_ts;
    long long stop_ts;
    long long elapsed_time;
    long lElapsedTime;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; // Start time

    // Create threads
    for (i = 0; i < NTHREADS; i++)
    {
        numberArray[i] = i;
        if(parent == getpid())
        {
            pid = fork();
        } 
        if(pid == 0)
        {
            tfunc((void *) &numberArray[i]);
            exit(0);
        }
    }

    double sum = 0;
    for (i = 0; i < NTHREADS; i++) {
        wait(&status);
        status = status >> 8;

        char filename[8];
        sprintf(filename, "result%d", status);
        FILE *fp = fopen(filename, "r");
        while(fgets(line, sizeof line, fp) != NULL)
            threadResults[status] = atof(line);
        fclose(fp);
        remove(filename);
    }

    for (i = 0; i < NTHREADS; i++)
    {
        result += threadResults[i];
    }


	printf("Result for %d: %f\n", ITERATIONS, result);

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // End time

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TOTAL TIME: %d seconds\n", (int)elapsed_time);
}
