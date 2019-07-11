#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vdisk.h>
#include <seclog.h>

#define LINESIZE 16
#define SECSIZE 512

int main(int argc,char *argv[])
{
	int drive;
	int logunit;
	int nseclog;
	int fd;
	unsigned char buffer[SECSIZE];
	int offset;
	int i,j,r;
	unsigned char c;

	if(argc==3)
	{	
		logunit=atoi(argv[1]);
		nseclog=atoi(argv[2]);

		printf("Desplegando Unidad lógica=%d, Seclog=%d\n",logunit,nseclog);
		
	}
	else	
	{
		fprintf(stderr,"Error en los argumentos\n");
		exit(1);
	}

	if(vdreadseclog(logunit,nseclog,buffer)==-1)
	{
		fprintf(stderr,"Error al abrir disco virtual\n");
		exit(1);
	}

	for(i=0;i<SECSIZE/LINESIZE;i++)
	{
		printf("\n %3X -->",i*LINESIZE);
		for(j=0;j<LINESIZE;j++)
		{
			c=buffer[i*LINESIZE+j];
			printf("%2X ",c);
		}
		printf("  |  ");
		for(j=0;j<LINESIZE;j++)
		{
			c=buffer[i*LINESIZE+j]%256;
			if(c>0x1F && c<127)
				printf("%c",c);
			else
				printf(".");
		}
	}
	printf("\n");
}

