#include <scheduler.h>
#include <stdio.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

#define PRIORITYNUMBER 5

QUEUE ready[PRIORITYNUMBER];
QUEUE waitinginevent[MAXTHREAD];

int currentPriority = 0;

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

// Funcion que determina si hay más procesos pendientes en alguna otra cola
int moreUpcoming(int process){
	int i,j;
	for(i = 0; i < PRIORITYNUMBER; i++){
		if(!_emptyq(&ready[i])){
			int tail = ready[i].tail;
			if(ready[i].elements[tail] != -1 && ready[i].elements[tail] != process){
				return 1;
			}
		}
	}
	return 0;
}

// Funcion de verificacion para imprimir las colas en consola (QUITAR AL ENTREGAR)
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
}

// Función que determina la prioridad en la que se encuentra un proceso en determinado momento
int findPriority(int process){
	printf("------\n");
	int i,j;
	for(i = 0; i < PRIORITYNUMBER; i++){
			int head = ready[i].head;
			int tail = ready[i].tail;
			if (tail <= head) {
				for(j = tail; j <= head; j++){
					printf("Comparacion: ELemento:%d\t Proceso:%d\n",ready[i].elements[j],process);
					if(ready[i].elements[j] == process){ 
						return i;
					}		
				}
			}
			else {
					for(j = head; j <= tail; j++){ // En cada cola se revisan los valores en el rango de la cola a la cabeza (por como fue programado)
						if(j > head && j < tail) {
							if(ready[i].elements[j] == process){ 
								return i;
							}		
						}
					}
			}
	}
	return -1;
}

void scheduler(int arguments)
{
	if(!filledQueues){
		fillQueues();
		filledQueues = 1;
	}

	int old,next;
	int changethread=0;
	int waitingthread=0;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos en prioridad 0
		threads[callingthread].status=READY;
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
		//printf("TIMER: %d, Calling Thread: %d\n", event, callingthread);
		threads[callingthread].status = READY;
		//Bandera de control del encolado
		int enqueueFlag = 0;

		if(moreUpcoming(callingthread)){
			currentPriority = findPriority(callingthread);
			if(currentPriority < (PRIORITYNUMBER -1)){
				_enqueue(&ready[++currentPriority],callingthread);
				printf("IF 1 Proceso %d en prioridad %d\n",callingthread,currentPriority);
			}else{
				_enqueue(&ready[4],callingthread);
				printf("IF 2 Proceso %d en prioridad %d\n",callingthread,currentPriority);
			}
			changethread = 1;
			//printQueues();
		}
	}
	
	if(event==UNBLOCKTHREAD)
	{
		threads[callingthread].status=READY;
		//	_enqueue(&ready[currentPriority],callingthread);
		currentPriority = 0;
		_enqueue(&ready[0], callingthread);
	}

	
	if(changethread)
	{
		old=currthread;
		// Determinar el proceso que se debe extraer (el de la prioridad más alta encontrado)
		int i;
		for(i = 0; i < PRIORITYNUMBER; i++)
		{
		//	printf("Cola %d, estado: %d, Tail: %d, Head: %d\n", i, _emptyq(&ready[i]), ready[i].tail, ready[i].head);
			if(!_emptyq(&ready[i]))
			{
				next = _dequeue(&ready[i]); // Cambiar prioridades
				printf("Cambiando el proceso %d con prioridad %d para que entre el proceso %d en prioridad %d\n",old,currentPriority,next,i);
				break;
			}
		}

		// Luego de esto, hacer el cambio de prioridad y actualizar el valor de currentPriority
		threads[next].status=RUNNING;
		_swapthreads(old,next);
		printQueues();
	}

}
