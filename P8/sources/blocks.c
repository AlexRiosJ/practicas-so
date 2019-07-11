#include <bootsector.h>
#include <seclog.h>
#include <blocks.h>


// **********************************************************************************
// Lectura y escritura de bloques
// **********************************************************************************

int writeblock(int block,char *buffer)
{
	int result;
	int i;
	int inicio_area_datos;
	int sec_x_bloque;
	
	sec_x_bloque=secs_per_block();

	inicio_area_datos=sec_data_area();

	for(i=0;i<sec_x_bloque;i++)
		vdwriteseclog(0,inicio_area_datos+(block-1)*sec_x_bloque+i,buffer+512*i);
	return(1);	
}

int readblock(int block,char *buffer)
{
	int result;
	int i;
	int inicio_area_datos;
	int sec_x_bloque;
	
	sec_x_bloque=secs_per_block();
	
	inicio_area_datos=sec_data_area();

	for(i=0;i<sec_x_bloque;i++)
		vdreadseclog(0,inicio_area_datos+(block-1)*sec_x_bloque+i,buffer+512*i);
	return(1);	
}
