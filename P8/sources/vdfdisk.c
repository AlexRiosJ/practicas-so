#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <vdisk.h>
#include <mbr.h>

// Crear particiones en un disco virtual
// Para la práctica solo vamos a crear una partición de todo el disco
int main()
{
	struct MBR mbr;
	
	int secfinic=2;
	int cilinic=0;
	int supinic=0;

	int secffin=SECTORS;		// 27
	int cilfin=CYLINDERS-1;		// 199
	int supfin=HEADS-1;			// 7

	// Llenar la estructura mbr con 0s
	memset(&mbr,0,512);
	
	mbr.partition[0].chs_begin[0]=supinic;
	mbr.partition[0].chs_begin[1]=(cilinic & 0x300)>>2 | secfinic & 0x3F;
	mbr.partition[0].chs_begin[2]=cilinic & 0xFF;

	mbr.partition[0].chs_end[0]=supfin;
	mbr.partition[0].chs_end[1]=(cilfin & 0x300)>>2 | secffin & 0x3F;
	mbr.partition[0].chs_end[2]=cilfin & 0xFF;

	// Escribir la tabla de particiones en el SF=1, sup=0, cil=0
	vdwritesector(0,0,0,1,1, (char *) &mbr);
}
