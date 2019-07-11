#pragma pack(1)

struct INODE {
	char name[22];
	unsigned short uid;
	unsigned short gid;
	unsigned short perms;
	unsigned int datetimecreat;
	unsigned int datetimemodif;
	unsigned int size;
	unsigned short blocks[10];
	unsigned short indirect;
	unsigned short indirect2;
};

int setninode(int num, char *filename,unsigned short atribs, int uid, int gid);
int searchinode(char *filename);
int removeinode(int numinode);

#define MAXENTRIES 24