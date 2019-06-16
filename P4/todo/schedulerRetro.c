#include <scheduler.h>
#include <stdio.h>
#include <stdlib.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

#define PRIORITYNUMBER 5

QUEUE ready[PRIORITYNUMBER];
QUEUE waitinginevent[MAXTHREAD];

int currentPriority = 0;

int callPriority = 0;

int filledQueues = 0;

void fillQueues()
{
	int i, j;
	for (i = 0; i < PRIORITYNUMBER; i++)
	{
		for (j = 0; j < MAXTHREAD; j++)
		{
			ready[i].elements[j] = -1;
		}
	}
	for (i = 0; i < MAXTHREAD; i++)
	{
		char filename[10];
		sprintf(filename, "process%d", i);
		FILE *fp = fopen(filename, "w");
		fprintf(fp, "%d\n", -1);
		fclose(fp);
	}
}

// Funcion que determina si hay más procesos pendientes en alguna otra cola
int moreUpcoming(int process)
{
	int i, j;
	for (i = 0; i < PRIORITYNUMBER; i++)
	{
		if (!_emptyq(&ready[i]))
		{
			int head = ready[i].head;
			int tail = ready[i].tail;
			int j;
			if (tail < head)
			{
				for (j = tail; j < head; j++)
				{
					if (ready[i].elements[j] != -1 && ready[i].elements[j] != process)
					{
						return 1;
					}
				}
			}
			else
			{
				for (j = 0; j < MAXTHREAD; j++)
				{
					if (j < head || j >= tail)
					{
						if (ready[i].elements[j] != -1 && ready[i].elements[j] != process)
						{
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

void scheduler(int arguments)
{
	if (!filledQueues)
	{
		fillQueues();
		filledQueues = 1;
	}

	int old, next;
	int changethread = 0;
	int waitingthread = 0;

	int event = arguments & 0xFF00;
	int callingthread = arguments & 0xFF;

	if (event == NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos en prioridad 0
		threads[callingthread].status = READY;
		_enqueue(&ready[0], callingthread);
		currentPriority = 0; // Comienza con prioridad 0
	}

	if (event == BLOCKTHREAD)
	{

		threads[callingthread].status = BLOCKED;
		_enqueue(&waitinginevent[blockevent], callingthread);
		changethread = 1;
	}

	if (event == ENDTHREAD)
	{
		threads[callingthread].status = END;
		changethread = 1;
	}

	if (event == TIMER)
	{
		threads[callingthread].status = READY;

		char filename[10];
		char line[10];
		sprintf(filename, "process%d", callingthread);
		FILE *fp = fopen(filename, "r");
		while (fgets(line, sizeof line, fp) != NULL)
			currentPriority = atoi(line);
		fclose(fp);

		if (moreUpcoming(callingthread))
		{
			if (currentPriority < (PRIORITYNUMBER - 1))
			{
				_enqueue(&ready[currentPriority + 1], callingthread);
				callPriority = currentPriority + 1;
			}
			else
			{
				_enqueue(&ready[currentPriority], callingthread);
				callPriority = currentPriority;
			}
			changethread = 1;
		}
	}

	if (event == UNBLOCKTHREAD)
	{
		threads[callingthread].status = READY;
		_enqueue(&ready[currentPriority], callingthread);
	}

	if (changethread)
	{
		old = currthread;

		char filename[10];
		sprintf(filename, "process%d", old);
		FILE *fp = fopen(filename, "w");
		fprintf(fp, "%d\n", callPriority);
		fclose(fp);

		// Luego de esto, hacer el cambio de prioridad y actualizar el valor de currentPriority
		if (!_emptyq(&ready[currentPriority]))
			next = _dequeue(&ready[currentPriority]);
		else
		{
			next = _dequeue(&ready[++currentPriority]);
		}

		threads[next].status = RUNNING;
		_swapthreads(old, next);
	}
}