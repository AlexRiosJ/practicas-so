#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "semaphores.h"

							
#define CICLOS 10

char *pais[3]={"Peru","Bolvia","Colombia"};

int semaforo;

void proceso(int i)
{
	int k;

	for(k=0;k<CICLOS;k++)
	{
		semwait(semaforo);	
		// Inicia sección Crítica
		printf("Entra %s\t",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("-\t %s Sale\n",pais[i]);
		// Termina sección Crítica
		semsignal(semaforo);
		
		// Espera aleatoria fuera de la sección crítica
		sleep(rand()%3);
	}
    exit(0);   // Termina el proceso
}

int main()
{
	int pid;
    int status;
	int args[3];
	int i;

	srand(getpid());

    // Inicializando un semáforo desde el SO para recibir a los procesos
	semaforo = seminit(0x1234,1);

	// Crea los hilos
	for(i=0;i<3;i++)
	{
        // Crea un nuevo proceso hijo que ejecuta la función proceso()
		pid = fork();
        if(pid == 0)
            proceso(i);
	}

	// Espera que terminen los hilos
	for(i=0;i<3;i++)
        pid = wait(&status);
		
    // Borrando el semáforo creado
	semdelete(semaforo);
}