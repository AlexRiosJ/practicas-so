#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
				
#define CICLOS 10

char *pais[3]={"Peru","Bolvia","Colombia"};


typedef struct {
	long msg_type;		// Tipo de mensaje, debe ser long
	char mensaje[100];	// Contenido
} MSGTYPE; 

int msgqid;

void send(int proceso, char* mensaje){
	MSGTYPE m;
	m.msg_type=1;
	sprintf(m.mensaje,"%s", mensaje);
	msgsnd(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),0);
}

void receive(){
	MSGTYPE m;	// Donde voy a recibir el mensaje
	int retval;	
	do
	{
		retval=msgrcv(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),1,0);
		sleep(3);
	}while(strcmp(m.mensaje,"Libre")!=0); // Hasta que el mensaje sea "Libre"
}

void proceso(int i)
{
	int k;

	for(k=0;k<CICLOS;k++)
	{
		// Espera el mensaje entregado
		receive();

		// Inicia sección Crítica
		printf("Entra %s\t",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("-\t %s Sale\n",pais[i]);
		// Termina sección Crítica

		// Recepción del mensaje
		send(k,"Libre");	

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

	// Crear un buzón o cola de mensajes
	msgqid=msgget(0x1234,0666|IPC_CREAT);
	
	send(0,"Libre");

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
		
    // Borrando la cola de mensajes creada
	msgctl(msgqid,IPC_RMID,NULL);
}