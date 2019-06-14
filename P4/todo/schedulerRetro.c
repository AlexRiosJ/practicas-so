#include <scheduler.h>
#include <stdio.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

#define PRIORITYNUMBER 5

QUEUE ready[PRIORITYNUMBER];
QUEUE waitinginevent[MAXTHREAD];

int currentPriority;

void scheduler(int arguments)
{
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
		threads[callingthread].status = READY;
		// Si hay más de un elemento en la lista de prioridad 0 entonces cambia la prioridad del hilo actual
		// Encola en la siguiente prioridad:
		if(!_emptyq(&ready[currentPriority]) && currentPriority < PRIORITYNUMBER - 1){
			currentPriority++;
			_enqueue(&ready[currentPriority], callingthread);
			changethread = 1;
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
		int oldPriority;
		// Determinar el proceso que se debe extraer (el de la prioridad más alta encontrado)
		int i;
		for(i = 0; i < PRIORITYNUMBER; i++)
		{
			
			if(!_emptyq(&ready[i]))
			{
				printf("La cola %d no esta vacia, %d, %d\n", i, ready[i].head, ready[i].tail);
				next=_dequeue(&ready[i]); // Cambiar prioridades
				break;
			}
		}

		// Luego de esto, hacer el cambio de prioridad y actualizar el valor de currentPriority
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}

}
