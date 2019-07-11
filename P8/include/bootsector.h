#pragma pack(1)

struct SECBOOT {
	char jump[4];
	// Parámetros que describen la partición.
	char nombre_disco[8];
	unsigned char sec_res;
	unsigned char sec_mapa_bits_nodos_i;
	unsigned char sec_mapa_bits_bloques;
	unsigned short sec_tabla_nodos_i;
	unsigned short sec_log_unidad;
	unsigned char sec_x_bloque;
	unsigned char heads;
	unsigned char cyls;
	unsigned char secfis;	
	// código de boot
	char restante[489];
};

int sec_data_area();
int sec_inodes();
int sec_bitmap();
int sec_bitmap_inodes();
int secs_inode_table();
int secs_bitmap();
int secs_per_block();