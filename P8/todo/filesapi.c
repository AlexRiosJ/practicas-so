#include <stddef.h>
#include <bootsector.h>
#include <seclog.h>
#include <bitmaps.h>
#include <blocks.h>
#include <inode.h>
#include <filesys.h>
#include <filesapi.h>
#include <users.h>

extern struct SECBOOT sb;
extern int sb_in_mem;

extern struct INODE inode[MAXENTRIES];
extern int nodos_i_en_memoria;

// Funciones para crear, abrir y eliminar archivos

extern int openfiles_inicializada;
extern struct OPENFILES openfiles[16];



int vdopen(char *filename,unsigned short mode)
{
	int numinode;
	int i;
	unsigned short currblock;


	// Ver si ya existe el archivo
	// Si no existe regresa con un error
	numinode=searchinode(filename);
	if(numinode==-1)
		return(-1);

	// Si no está inicializada la tabla de archivos abiertos inicialízala
	if(!openfiles_inicializada)
	{
		for(i=3;i<16;i++)
		{
			openfiles[i].inuse=0;
			openfiles[i].currbloqueenmemoria=-1;
		}
		openfiles_inicializada=1;
	}

	// Buscar si hay lugar en la tabla de archivos abiertos
	// Si no hay lugar, regresa -1
	i=3;
	while(openfiles[i].inuse && i<16)
		i++;

	if(i>=16)
		return(-1);

	openfiles[i].inuse=1;
	openfiles[i].inode=numinode;
	openfiles[i].currpos=0;

	// Si hay apuntador indirecto, leerlo en el buffer
	if(inode[numinode].indirect!=0)
		readblock(inode[numinode].indirect,(char *) openfiles[i].buffindirect);

	// Cargar el buffer con el bloque actual del archivo (primer bloque)
	currblock=*currpostoptr(i);
	readblock(currblock,openfiles[i].buffer);
	return(i);
}


int vdcreat(char *filename,unsigned short perms)
{
	int numinode;
	int i;

	// Ver si ya existe el archivo
	numinode=searchinode(filename);
	if(numinode==-1)
	{
		// Buscar un inodo en blanco en el mapa de bits (nodos i)
		numinode=nextfreeinode();
		if(numinode==-1)
		{
			return(-1); // No hay espacio para más archivos
		}
	} else	// Elimina el inodo
		removeinode(numinode);


	// Escribir el archivo en el inodo encontrado

	setninode(numinode,filename,perms,getuid(),getgid());
	assigninode(numinode);


	if(!openfiles_inicializada)
	{
		for(i=3;i<16;i++)
		{
			openfiles[i].inuse=0;
			openfiles[i].currbloqueenmemoria=-1;
		}
		openfiles_inicializada=1;
	}

	// Buscar si hay lugar en la tabla de archivos abiertos
	// Si no hay lugar, regresa -1
	i=3;
	while(openfiles[i].inuse && i<16)
		i++;

	if(i>=16)
		return(-1);

	openfiles[i].inuse=1;
	openfiles[i].inode=numinode;
	openfiles[i].currpos=0;
	return(i);
}


int vdunlink(char *filename)
{
	int numinode;
	int i;

	// Busca el inodo del archivo
	numinode=searchinode(filename);
	if(numinode==-1)
		return(-1); // No existe

	removeinode(numinode);
}




int vdseek(int fd, int offset, int whence)
{
	unsigned short oldblock,newblock;

	// Si no está abierto regresa error
	if(openfiles[fd].inuse==0)
		return(-1);

	oldblock=*currpostoptr(fd);
		
	if(whence==0) // A partir del inicio
	{
		if(offset<0 || 
		   openfiles[fd].currpos+offset>inode[openfiles[fd].inode].size)
			return(-1);
		openfiles[fd].currpos=offset;

	} else if(whence==1) // A partir de la posición actual
	{
		if(openfiles[fd].currpos+offset>inode[openfiles[fd].inode].size ||
		   openfiles[fd].currpos+offset<0)
			return(-1);
		openfiles[fd].currpos+=offset;

	} else if(whence==2) // A partir del final
	{
		if(offset>inode[openfiles[fd].inode].size ||
		   openfiles[fd].currpos-offset<0)
			return(-1);
		openfiles[fd].currpos=inode[openfiles[fd].inode].size-offset;
	} else
		return(-1);

	newblock=*currpostoptr(fd);
	
	if(newblock!=oldblock)
	{
		writeblock(oldblock,openfiles[fd].buffer);
		readblock(newblock,openfiles[fd].buffer);
		openfiles[fd].currbloqueenmemoria=newblock;
	}

	return(openfiles[fd].currpos);
}


int vdwrite(int fd, char *buffer, int bytes)
{
	int currblock;
	int currinode;
	int cont=0;
	int sector;
	int inicio_nodos_i;
	int i;
	int result;
	unsigned short *currptr;

	// Si no está abierto, regresa error
	if(openfiles[fd].inuse==0)
		return(-1);

	// Checar si el sector de boot está en memoria
	if(!sb_in_mem)
	{
		// Si no está en memoria, cárgalo
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}
	inicio_nodos_i=sb.sec_res+sb.sec_mapa_bits_nodos_i+sb.sec_mapa_bits_bloques;

	currinode=openfiles[fd].inode;

	while(cont<bytes)
	{
		// Obtener la dirección de donde está el bloque que corresponde
		// a la posición actual
		currptr=currpostoptr(fd);
		if(currptr==NULL)
			return(-1);
	
		currblock=*currptr;

		// Si el bloque está en blanco, dale uno
		if(currblock==0)
		{
			currblock=nextfreeblock();
			// El bloque encontrado ponerlo en donde
			// apunta el apuntador al bloque actual
			*currptr=currblock;
			assignblock(currblock);	
			
			// Escribir el sector de la tabla de nodos i
			// En el disco
			sector=(currinode/8)*8;
			result=vdwriteseclog(0,inicio_nodos_i+sector,(char *) &inode[sector*8]);
		}

		// Si el bloque de la posición actual no está en memoria
		// Lee el bloque al buffer del archivo
		if(openfiles[fd].currbloqueenmemoria!=currblock)
		{
			// Leer el bloque actual hacia el buffer que
			// está en la tabla de archivos abiertos
			readblock(currblock,openfiles[fd].buffer);			
			openfiles[fd].currbloqueenmemoria=currblock;
		}

		// Copia el caracter al buffer
		openfiles[fd].buffer[openfiles[fd].currpos%1024]=buffer[cont];

		// Incrementa posición
		openfiles[fd].currpos++;

		// Si la posición es mayor que el tamaño, modifica el tamaño
		if(openfiles[fd].currpos>inode[currinode].size)
			inode[openfiles[fd].inode].size=openfiles[fd].currpos;

		// Incrementa el contador
		cont++;

		// Si se llena el buffer, escríbelo
		if(openfiles[fd].currpos%1024==0)
			writeblock(currblock,openfiles[fd].buffer);
	}
	return(cont);
} 

int vdread(int fd, char *buffer, int bytes)
{
	// Esta es la función a realizar
} 



int vdclose(int fd)
{
	int currblock;
	int currinode;
	int inicio_nodos_i;
	int i;
	int result;

	// Checar si el sector de boot está en memoria
	if(!sb_in_mem)
	{
		// Si no está en memoria, cárgalo
		result=vdreadseclog(0,0,(char *) &sb);
		sb_in_mem=1;
	}
	inicio_nodos_i=sb.sec_res+sb.sec_mapa_bits_nodos_i+sb.sec_mapa_bits_bloques;
	
	// Escribe lo que hay en el buffer
	currinode=openfiles[fd].inode;
	// currblock=inode[currinode].blocks[openfiles[fd].currpos/1024];
	currblock=*currpostoptr(fd);
	writeblock(currblock,openfiles[fd].buffer);
	
	// Escribe el bloque indirecto
	writeblock(inode[currinode].indirect,(char *) openfiles[fd].buffindirect);

	// Escribe el nodo i que está en memoria a los sectores del disco
	for(i=0;i<sb.sec_tabla_nodos_i;i++)
		result=vdwriteseclog(0,inicio_nodos_i+i,(char *)&inode[i*8]);

	openfiles[fd].inuse=0;
	openfiles[fd].inode=-1;
	openfiles[fd].currpos=0;

}
