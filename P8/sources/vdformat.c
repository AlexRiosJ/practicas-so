#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vdisk.h>
#include <mbr.h>
#include <bootsector.h>
#include <seclog.h>
#include <bitmaps.h>


extern struct MBR mbr;
extern int mbr_in_mem;

int main(int argc, char *argv[])
{
	struct SECBOOT sb;
	char buffer[512];
	int logunit=0;
	int secfinic,cilinic,supinic;
	int secffin,cilfin,supfin;
	int result;
	int slad;
	int i;

	// Validar que se reciba el argumento de la linea de comandos	
	if(argc!=2)
	{
		fprintf(stderr,"Error en los argumentos\n\tvdformat unidad_lógica\n");
		exit(1);
	}
	logunit=atoi(argv[1]);	// Convertir el argumento unidad lógica a número
	
	// Obtener los datos de la partición a formatear de la tabla de particiones
	if(!mbr_in_mem)
	{
		// Leer el sector físico donde está el MBR
		result=vdreadsector(0,0,0,1,1, (char *) &mbr);
	}
	
	// Obtener los datos de donde inicia y donde termina la partición
	supinic=mbr.partition[logunit].chs_begin[0];
	secfinic=mbr.partition[logunit].chs_begin[1] & 0x3F;
	cilinic=((mbr.partition[logunit].chs_begin[1] & 0xC0)<<2) + mbr.partition[logunit].chs_begin[2];

	supfin=mbr.partition[logunit].chs_end[0];
	secffin=mbr.partition[logunit].chs_end[1] & 0x3F;
	cilfin=((mbr.partition[logunit].chs_end[1] & 0xC0)<<2) + mbr.partition[logunit].chs_end[2];

	// Limpiar el sector de boot
	memset(&sb,0,512);
	
	// Poner los valores del formato en el sector de boot
	strcpy(sb.nombre_disco,"FSYS2019");
	sb.sec_res=1;		// 1 sector reservado como sector de boot de la partición
	sb.sec_mapa_bits_nodos_i=1;
	sb.sec_mapa_bits_bloques=6;
	sb.sec_tabla_nodos_i=3;	
	sb.sec_x_bloque=2;	// Sectores por bloque
	sb.heads=HEADS;
	sb.cyls=cilfin - cilinic + 1;	// Total de cilindros que tiene la partición
	sb.secfis=SECTORS;		
	sb.sec_log_unidad=sb.heads*sb.cyls*sb.secfis;	
	// Escribir en el sector lógico 0 el sector de boot que contiene
	// la tabla que describe el sistema de archivos de la partición
	vdwriteseclog(0,0,(char *) &sb);
	slad=sb.sec_res+sb.sec_mapa_bits_nodos_i+sb.sec_mapa_bits_bloques+sb.sec_tabla_nodos_i;

	// Llenar con 0s los sectores que son:
	//	Mapa de bits del área de nodos i
	// 	Mapa de bits del área de datos
	//	Nodos i
	for(i=sb.sec_res=1;i<slad;i++)
	{
		memset(buffer,0,512);
		vdwriteseclog(0,i,buffer);
	}

	// Escribir 1 en el primer bit del mapa de bits del área de datos
	// Bloque 1 no debe ser asignado
	assignblock(0);
}