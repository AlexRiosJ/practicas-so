// Tabla de archivos abiertos
struct OPENFILES {
	int inuse;
	unsigned short inode;
	int currpos;
	int currbloqueenmemoria;
	char buffer[1024];
	unsigned short buffindirect[512];
};

unsigned short *postoptr(int fd,int pos);
unsigned short *currpostoptr(int fd);