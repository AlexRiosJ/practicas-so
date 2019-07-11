// *************************************************************************
// Para el mapa de bits del área de nodos i
// *************************************************************************

int isinodefree(int inode);
int nextfreeinode();
int assigninode(int inode);
int unassigninode(int inode);

// ******************************************************************************
// Para el mapa de bits del área de de datos
// ******************************************************************************

int isblockfree(int block);
int nextfreeblock();
int assignblock(int block);
int unassignblock(int block);