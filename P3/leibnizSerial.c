#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

#define ITERATIONS 2000000000

// Compile with: gcc -o leibniz leibniz.c -lm

int main(){
	
	int i;
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Start time
	
	double result = 0;
	for(i = 0; i < ITERATIONS; i++){
		result += (pow(-1,i) / (2 * i + 1));
	}
	
	printf("Result for %d: %f\n",ITERATIONS,result);
	
	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // End time

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TOTAL TIME: %d seconds\n",(int) elapsed_time);
}