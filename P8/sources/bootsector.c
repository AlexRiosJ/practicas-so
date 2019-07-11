#include <seclog.h>
#include <bootsector.h>

struct SECBOOT sb;
int sb_in_mem=0;


int sec_data_area()
{
	int result;
	
	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}
	return(sb.sec_res+sb.sec_mapa_bits_nodos_i +sb.sec_mapa_bits_bloques+sb.sec_tabla_nodos_i);
}

int sec_inodes()
{
	int result;
	
	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}
	return(sb.sec_res+sb.sec_mapa_bits_nodos_i +sb.sec_mapa_bits_bloques);
}

int sec_bitmap()
{
	int result;
	
	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}
	return(sb.sec_res+sb.sec_mapa_bits_nodos_i);
}

int sec_bitmap_inodes()
{
	int result;
	
	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}
	return(sb.sec_res+sb.sec_mapa_bits_nodos_i);
}

// Sectores que contiene la tabla de inodos
int secs_inode_table()
{
	int result;
	
	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}

	return(sb.sec_tabla_nodos_i);
}

// Sectores con los mapas de bits de bloques
int secs_bitmap()
{
	int result;
	
	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}

	return(sb.sec_mapa_bits_bloques);
}

int secs_per_block()
{
	int result;
	
	if(!sb_in_mem)
	{
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}

	return(sb.sec_x_bloque);
}

