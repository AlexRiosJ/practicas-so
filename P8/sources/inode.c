#include <stdio.h>
#include <string.h>
#include <bootsector.h>
#include <bitmaps.h>
#include <blocks.h>
#include <seclog.h>
#include <datetime.h>
#include <inode.h>

int nodos_i_en_memoria=0;
struct INODE inode[MAXENTRIES];


int setninode(int num, char *filename,unsigned short atribs, int uid, int gid)
{
	int i;

	int result;
	int inicio_nodos_i;

	inicio_nodos_i=sec_inodes();
	if(!nodos_i_en_memoria)
	{
		for(i=0;i<secs_inode_table();i++)
			result=vdreadseclog(0,inicio_nodos_i+i,(char *) &inode[i*8]);

		nodos_i_en_memoria=1;
	}

	
	strncpy(inode[num].name,filename,20);

	if(strlen(inode[num].name)>19)
	 	inode[num].name[19]='\0';

	inode[num].datetimecreat=currdatetimetoint();
	inode[num].datetimemodif=currdatetimetoint();
	inode[num].uid=uid;
	inode[num].gid=gid;
	inode[num].perms=atribs;
	inode[num].size=0;
	
	for(i=0;i<10;i++)
		inode[num].blocks[i]=0;

	inode[num].indirect=0;
	inode[num].indirect2=0;

	// Optimizar la escritura escribiendo solo el sector lógico que
	// corresponde al inodo que estamos asignando.
	// i=num/8;
	// result=vdwriteseclog(inicio_nodos_i+i,&inode[i*8]);
	for(i=0;i<secs_inode_table();i++)
		result=vdwriteseclog(0,inicio_nodos_i+i,(char *) &inode[i*8]);

	return(num);
}

int searchinode(char *filename)
{
	int i;
	int free;
	int result;
	int inicio_nodos_i;

	inicio_nodos_i=sec_inodes();
	
	if(!nodos_i_en_memoria)
	{
		for(i=0;i<secs_inode_table();i++)
				result=vdreadseclog(0,inicio_nodos_i+i,(char *) &inode[i*8]);

		nodos_i_en_memoria=1;
	}
	
	if(strlen(filename)>19)
	  	filename[19]='\0';

	i=0;
	while(strcmp(inode[i].name,filename) && i<MAXENTRIES)
		i++;

	if(i>=MAXENTRIES)
		return(-1);
	else
		return(i);
}

int removeinode(int numinode)
{
	int i;

	unsigned short temp[512];

	for(i=0;i<10;i++)
		if(inode[numinode].blocks[i]!=0)
			unassignblock(inode[numinode].blocks[i]);

	if(inode[numinode].indirect!=0)
	{
		// Leer el bloque
		readblock(inode[numinode].indirect,(char *) temp);
		for(i=0;i<512;i++)
			if(temp[i]!=0)
				unassignblock(temp[i]);
		unassignblock(inode[numinode].indirect);
		inode[numinode].indirect=0;
	}
	unassigninode(numinode);
	return(1);
}
