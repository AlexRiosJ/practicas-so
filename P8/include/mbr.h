#pragma pack(1)

struct PARTITION {
	unsigned char drive_status;
	unsigned char chs_begin[3];	// Cilinder Head Sector inicial de la partición
	unsigned char partition_type;
	unsigned char chs_end[3];	// Cilinder Head Sector final de la partición
	int lba;			
	int secs_partition;	
};

// Lo que vamos a escribir en el primer sector del disco
// Debe medir 512 bytes
struct MBR {
	char bootstrap_code[446];	// Código de arranque 
	struct PARTITION partition[4];	// Arreglo de 4 entradas, cada una es una partición
	short boot_signature;
};