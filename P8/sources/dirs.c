#include <stddef.h>
#include <string.h>
#include <bootsector.h>
#include <bitmaps.h>
#include <seclog.h>
#include <inode.h>
#include <dirs.h>


extern struct SECBOOT sb;
extern int sb_in_mem;

extern int nodos_i_en_memoria;
extern struct INODE inode[MAXENTRIES];

/*************************************************************
/* Manejo de directorios
 *************************************************************/
VDDIR dirs[2]={-1,-1};
struct vddirent current;


VDDIR *vdopendir(char *path)
{
	int i=0;
	int result;
	int inicio_nodos_i;

	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}
	inicio_nodos_i=sb.sec_res+sb.sec_mapa_bits_nodos_i+sb.sec_mapa_bits_bloques;

	if(!nodos_i_en_memoria)
	{
		for(i=0;i<sb.sec_tabla_nodos_i;i++)
			result=vdreadseclog(0,inicio_nodos_i+i,(char *) &inode[i*8]);

		nodos_i_en_memoria=1;
	}

	if(strcmp(path,".")!=0)
		return(NULL);

	i=0;
	while(dirs[i]!=-1 && i<2)
		i++;

	if(i==2)
		return(NULL);

	dirs[i]=0;

	return(&dirs[i]);	
}

struct vddirent *vdreaddir(VDDIR *dirdesc)
{
	int i;
	int inicio_nodos_i;

	int result;
	
	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}
	
	inicio_nodos_i=sb.sec_res+sb.sec_mapa_bits_nodos_i+sb.sec_mapa_bits_bloques;

	if(!nodos_i_en_memoria)
	{
		for(i=0;i<sb.sec_tabla_nodos_i;i++)
			result=vdreadseclog(0,inicio_nodos_i+i,(char *) &inode[i*8]);

		nodos_i_en_memoria=1;
	}

	// Mientras no haya nodo i, avanza
	while(isinodefree(*dirdesc) && *dirdesc<4096)
		(*dirdesc)++;


	// Apunta a donde está el nombre en el inodo	
	current.d_name=inode[*dirdesc].name;

	(*dirdesc)++;

	if(*dirdesc>=4096)
		return(NULL);
	return( &current);	
}

int vdclosedir(VDDIR *dirdesc)
{
	(*dirdesc)=-1;
}
