#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

// FUNCIÓN DE HW PARA EL INTERCAMBIO DE PROCESOS 
#define atomic_xchg(A, B) __asm__ __volatile__( \
    " lock xchg %1,%0 ;\n"                      \
    : "=ir"(A)                                  \
    : "m"(B), "ir"(A));

// VARIABLES GLOBALES ESTABLECIDAS
#define CICLOS 10
char *pais[3] = {"Peru", "Bolvia", "Colombia"};

/* CÓDIGO AÑADIDO  DE LAS COLAS*/
typedef struct _QUEUE {
	int elements[CICLOS];
	int head;
	int tail;
} QUEUE;

void _initqueue(QUEUE *q)
{
	q->head=0;
	q->tail=0;
}
	

void _enqueue(QUEUE *q,int val)
{
	q->elements[q->head]=val;
	// Incrementa al apuntador
	q->head++;
	q->head=q->head%CICLOS;
}


int _dequeue(QUEUE *q)
{
	int valret;
	valret=q->elements[q->tail];
	// Incrementa al apuntador
	q->tail++;
	q->tail=q->tail%CICLOS;
	return(valret);
}

int _emptyq(QUEUE *q)
{
	return(q->head==q->tail);
}
/*  FIN DEL CÓDIGO AÑADIDO DE LAS COLAS */

/*  INICIO DEL CÓDIGO DEL SEMÁFORO */
typedef struct SEMAPHORE{
    unsigned int contador;
    unsigned int bloqueados;
    QUEUE cola_de_bloqueados;
}SEMAPHORE;

SEMAPHORE *semaforo;

/*  INICIO DE LAS FUNCIONES DEL SEMÁFORO */

void initsem(SEMAPHORE s,int cuenta){
    s.contador = cuenta;
    s.bloqueados = 0;
    _initqueue(&s.cola_de_bloqueados);
}

void waitsem(SEMAPHORE s){
    if(s.contador == 0){
        int procesoActual = getpid();
        _enqueue(&s.cola_de_bloqueados,procesoActual); // Se encola en la lista de bloqueados
        s.bloqueados++;
        // AÑADIR CÓDIGO PARA QUE SE DUERMA ESTE PROCESO
        // atomic_xchg();
    }else{
        s.contador--;
    }
}

void signalsem(SEMAPHORE s){
    if(s.bloqueados == 0){
        s.contador++;
    }else{
        int procesoPorDespertar = _dequeue(&s.cola_de_bloqueados);
        // AÑADIR CÓDIGO PARA QUE SE VUELVA A ACTIVAR ESE PROCESO
        // atomic_xchg();
        s.bloqueados--;
    }
}
/* FIN DEL CÓDIGO AÑADIDO */

void proceso(int i)
{
    int k;
    for (k=0;k<CICLOS;k++){
    // Llamada waitsem implementada en la parte 3
    waitsem(*semaforo);
    printf("Entra %s ",pais[i]);
    fflush(stdout);
    sleep(rand()%3);
    printf("-%s Sale\n",pais[i]);
    // Llamada waitsignal implementada en la parte 3
    signalsem(*semaforo);
    // Espera aleatoria fuera de la sección crítica
    sleep(rand()%3);
    }
    exit(0);
    // Termina el proceso
}

int main()
{
    int pid;
    int status;
    int shmid;
    int args[3];
    int i;
    void *thread_result;
    
    // Solicitar memoria compartida
    shmid = shmget(0x1234, sizeof(SEMAPHORE), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Error en la memoria compartida\n");
        exit(1);
    }
    
    // Conectar la variable a la memoria compartida
    semaforo = shmat(shmid, NULL, 0);
    if (semaforo == NULL)
    {
        perror("Error en el shmat\n");
        exit(2);
    }
    
    //Inicializando semáforo
    initsem(*semaforo,1);
    srand(getpid());
    
    for (i = 0; i < 3; i++)
    {
        // Crea un nuevo proceso hijo que ejecuta la función proceso()
        pid = fork();
        if (pid == 0)
            proceso(i);
    }
    
    for (i = 0; i < 3; i++)
        pid = wait(&status);
    
    // Eliminar la memoria compartida
    shmdt(semaforo);
}