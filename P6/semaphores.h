#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

void semwait(int semid)
{
	struct sembuf s;
	s.sem_num = 0; // Primero y único elemento del semáforo
	s.sem_op = -1; // Decrementa el semáforo
	s.sem_flg = SEM_UNDO;
	semop(semid, &s, 1);
	return;
}

void semsignal(int semid)
{
	struct sembuf s;
	s.sem_num = 0; // Primero y único elemento del semáforo
	s.sem_op = 1;  // Incrementa el semáforo
	s.sem_flg = SEM_UNDO;
	semop(semid, &s, 1);
	return;
}

int seminit(int idsem, int value)
{
	int semid;
	semid = semget(idsem, 1, 0666 | IPC_CREAT); // Solicitar un arreglo de semáforos al SO
	semctl(semid, 0, SETVAL, value);			// Inicializar el semáforo en el valor dado
	return (semid);								// retornar el id del semaforo creado
}

void semdelete(int semid)
{
	semctl(semid, 0, IPC_RMID, 0); // Eliminar el arreglo de semaforos
	return;
}