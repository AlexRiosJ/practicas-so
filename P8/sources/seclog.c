#include <vdisk.h>
#include <mbr.h>
#include <seclog.h>

struct MBR mbr;
int mbr_in_mem=0;

int vdreadseclog(int logunit, int seclog, char *buffer)
{
	int cil,sf,sup;
	int secfinic;
	int cilinic;
	int supinic;
	int base;
	int result;

	if(!mbr_in_mem)
	{
		// Leer el sector físico donde está el MBR
		result=vdreadsector(0,0,0,1,1, (char *) &mbr);
	}
	supinic=mbr.partition[logunit].chs_begin[0];
	secfinic=mbr.partition[logunit].chs_begin[1] & 0x3F;
	cilinic=(mbr.partition[logunit].chs_begin[1] & 0xC0)<<2 + mbr.partition[logunit].chs_begin[2];
	
	base=cilinic*(SECTORS*HEADS)+supinic*SECTORS+secfinic-1;
	seclog=seclog+base;
	
	sf = 1 + seclog % SECTORS;
	sup = (seclog / SECTORS) % HEADS;
	cil = seclog / (SECTORS * HEADS);
	
	result=vdreadsector(0,sup,cil,sf,1, buffer);
	return(result);
}

int vdwriteseclog(int logunit,int seclog, char *buffer)
{
	int cil,sf,sup;
	int secfinic;
	int cilinic;
	int supinic;
	int base;
	int result;
	
	if(!mbr_in_mem)
	{
		// Leer el sector físico donde está el MBR
		result=vdreadsector(0,0,0,1,1, (char *) &mbr);
	}
	supinic=mbr.partition[logunit].chs_begin[0];
	secfinic=mbr.partition[logunit].chs_begin[1] & 0x3F;
	cilinic=(mbr.partition[logunit].chs_begin[1] & 0xC0)<<2 + mbr.partition[logunit].chs_begin[2];
	
	base=cilinic*(SECTORS*HEADS)+supinic*SECTORS+secfinic-1;
	seclog=seclog+base;


	sf = 1 + seclog % SECTORS;
	sup = (seclog / SECTORS) % HEADS;
	cil = seclog / (SECTORS * HEADS);
 
	result=vdwritesector(0,sup,cil,sf,1, buffer);
	return(result);
}
