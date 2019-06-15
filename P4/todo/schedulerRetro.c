#include <scheduler.h>
#include <stdio.h>
#include <unistd.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

#define PRIORITYNUMBER 5

QUEUE ready[PRIORITYNUMBER];
QUEUE waitinginevent[MAXTHREAD];

int currentPriority;

int filledQueues = 0;

void fillQueues() {
	int i, j;
	for(i = 0; i < PRIORITYNUMBER; i ++)
	{
		for(j = 0; j < MAXTHREAD; j ++)
		{
			ready[i].elements[j] = -1;
		}
	}
}

void printQueues(){
	int i;
		for(i = 0; i < PRIORITYNUMBER; i++)
		{
			int j;
			printf("COLA %d:\t",i);
			for(j = 0; j < 10; j++) // para cada elemento
			{	
				printf("%d\t",ready[i].elements[j]);
			}
			printf("\n");
		}
			printf("Current: %d\n",currentPriority);
}

void scheduler(int arguments)
{
	sleep(1);
	if(!filledQueues){
		fillQueues();
		filledQueues = 1;
	}
	printf("###############################################################################################\n");
	int old,next;
	int changethread=0;
	int waitingthread=0;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos en prioridad 0
		threads[callingthread].status=READY;
		printf("Entra %d Head: %d, Tail: %d, HeadVal: %d, TailVal: %d\n",callingthread, ready[currentPriority].head, ready[currentPriority].tail, ready[currentPriority].elements[ready[currentPriority].head], ready[currentPriority].elements[ready[currentPriority].tail]);
		_enqueue(&ready[0],callingthread);
		printf("Entra %d Head: %d, Tail: %d, HeadVal: %d, TailVal: %d\n",callingthread, ready[currentPriority].head, ready[currentPriority].tail, ready[currentPriority].elements[ready[currentPriority].head], ready[currentPriority].elements[ready[currentPriority].tail]);		currentPriority = 0; // Comienza con prioridad 0
	}
	
	if(event==BLOCKTHREAD)
	{

		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);

		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
	}

	if (event == TIMER)
	{
		printf("TIMER: %d, Calling Thread: %d\n", event, callingthread);
		threads[callingthread].status = READY;
		//Bandera de control del encolado
		int enqueueFlag = 0;

		// Obteniendo la prioridad del proceso actual
		currentPriority = 0;
		int i;
		for(i = 0; i < PRIORITYNUMBER; i++) // Revisa cola por cola a ver si hay otro proceso con más prioridad
		{
			printf("TIMER - Cola %d, estado: %d, Tail: %d, Head: %d\n", i, _emptyq(&ready[i]), ready[i].tail, ready[i].head);
			// printf("PROCESO:%d\n",i);
			if(!_emptyq(&ready[i])){ // Si la cola no está vacía busca en sus elementos
				int head = ready[i].head;
				int tail = ready[i].tail;
				int j;
				if (tail <= head) {
					printf("Tail: %d, Head: %d\n", ready[i].tail, ready[i].head);
					for(j = tail; j <= head; j++){ // En cada cola se revisan los valores en el rango de la cola a la cabeza (por como fue programado)
						printf("P%d:\tElement:%d\tCurrent:%d\n",i,ready[i].elements[j],callingthread);
						if(ready[i].elements[j] == callingthread){ // Si se encuentra el proceso actual, esta es su cola de prioridad
							enqueueFlag = 1;
							currentPriority = i;
							// Si se puede pasar de prioridad, se aumenta
							// PONER FUNCION QUE DEVUELVA SI EXISTE OTRO PROCESO DIFERENTE EN LA COLA ACTUAL
							if(currentPriority < PRIORITYNUMBER - 1 /* && FUNCTION */){
								currentPriority++;
								_enqueue(&ready[currentPriority], callingthread);
								changethread = 1;
							}else{ // Si no se puede pasar de prioridad, sólo se mantiene
								_enqueue(&ready[currentPriority], callingthread);
								changethread = 1;
							}
							break;
						}
					}
				} else {
					printf("Tail: %d, Head: %d\n", ready[i].tail, ready[i].head);
					for(j = head; j <= tail; j++){ // En cada cola se revisan los valores en el rango de la cola a la cabeza (por como fue programado)
						if(j > head && j < tail) {
							printf("P%d:\tElement:%d\tCurrent:%d\n",i,ready[i].elements[j],callingthread);
							if(ready[i].elements[j] == callingthread){ // Si se encuentra el proceso actual, esta es su cola de prioridad
								enqueueFlag = 1;
								currentPriority = i;
								// Si se puede pasar de prioridad, se aumenta
								if(currentPriority < PRIORITYNUMBER - 1){
									currentPriority++;
									_enqueue(&ready[currentPriority], callingthread);
									changethread = 1;
								}else{ // Si no se puede pasar de prioridad, sólo se mantiene
									_enqueue(&ready[currentPriority], callingthread);
									changethread = 1;
								}
								break;
							}
						}
					}
				}
				printf("Cambio\n");
			}
		}

		// Encola en la prioridad actual:
		if(!enqueueFlag){
			printf("Entre a enqueueFlag... Prioridad: %d, CallingThread: %d\n", currentPriority, callingthread);
			_enqueue(&ready[currentPriority], callingthread);
			printf("Entre a enqueueFlag... Cola %d, estado: %d, Tail: %d, Head: %d\n", currentPriority, _emptyq(&ready[currentPriority]), ready[currentPriority].elements[ready[currentPriority].tail], ready[currentPriority].elements[ready[currentPriority].head]);
			printQueues();
		}
		

	}
	
	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
			_enqueue(&ready[currentPriority],callingthread);
	}

	
	if(changethread)
	{
		old=currthread;
		// Determinar el proceso que se debe extraer (el de la prioridad más alta encontrado)
		int i;
		for(i = 0; i < PRIORITYNUMBER; i++)
		{
			printf("Cola %d, estado: %d, Tail: %d, Head: %d\n", i, _emptyq(&ready[i]), ready[i].tail, ready[i].head);
			if(!_emptyq(&ready[i]))
			{
				next=_dequeue(&ready[i]); // Cambiar prioridades
				printf("Cambiando el proceso %d con prioridad %d para que entre el proceso %d en prioridad %d\n",old,currentPriority,next,i);
				break;
			}
		}
		// Luego de esto, hacer el cambio de prioridad y actualizar el valor de currentPriority
		threads[next].status=RUNNING;
		_swapthreads(old,next);
		printf("Cola %d, estado: %d, Tail: %d, Head: %d\n", i, _emptyq(&ready[i]), ready[i].tail, ready[i].head);
		// FUNCIÓN PARA VERIFICAR
		//printQueues();
	}

}
