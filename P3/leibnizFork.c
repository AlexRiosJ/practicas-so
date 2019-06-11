#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>

#define ITERATIONS 2000000000
#define NTHREADS 4

double result = 0;

void tfunc(void* args);

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
        printf("status: %d result = %lf\n", status, threadResults[status]);
        fclose(fp);
    }
}

void tfunc(void *args)
{
    int nthread = *((int*) args);
    int i;
    int inicio = nthread * (ITERATIONS / NTHREADS);
    int fin = (nthread + 1) * (ITERATIONS / NTHREADS);
    double threadResult = 0.0;

    printf("Thread num: %d, inicio: %d, fin: %d\n", nthread, inicio, fin);

    for (i = inicio; i < fin; i++)
    {
        if(i == inicio) printf("%d", nthread);
        threadResult += (pow(-1.0, i) / (2.0 * i + 1.0));
    }

    printf("%lf\n", threadResult);
    char filename[8];
    sprintf(filename, "result%d", nthread);
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "%lf\n", threadResult);
	fclose(fp);
	exit(nthread); // Sends shutdown state to init
}