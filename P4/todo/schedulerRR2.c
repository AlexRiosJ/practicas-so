#include <scheduler.h>

#define QUANTUMSIZE 2

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

int quantumCounter = 0;

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

void scheduler(int arguments)
{
	int old, next;
	int changethread = 0;
	int waitingthread = 0;

	int event = arguments & 0xFF00;
	int callingthread = arguments & 0xFF;

	if (event == NEWTHREAD)
	{
		quantumCounter = 0;
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status = READY;
		_enqueue(&ready, callingthread);
	}

	if (event == BLOCKTHREAD)
	{

		threads[callingthread].status = BLOCKED;
		_enqueue(&waitinginevent[blockevent], callingthread);

		changethread = 1;
	}

	if (event == ENDTHREAD)
	{
		quantumCounter = 0;
		threads[callingthread].status = END;
		changethread = 1;
	}

	if (event == UNBLOCKTHREAD)
	{
		quantumCounter = 0;
		threads[callingthread].status = READY;
		_enqueue(&ready, callingthread);
	}

	if (event == TIMER)
	{
		quantumCounter++;
		if (quantumCounter == 2)
		{
			threads[callingthread].status = READY;
			_enqueue(&ready, callingthread);
			changethread = 1;
			quantumCounter = 0;
		}
	}

	if (changethread)
	{
		quantumCounter = 0;
		old = currthread;
		next = _dequeue(&ready);

		threads[next].status = RUNNING;
		_swapthreads(old, next);
	}
}
