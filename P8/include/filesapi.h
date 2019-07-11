int vdopen(char *filename,unsigned short mode);
int vdcreat(char *filename,unsigned short perms);
int vdunlink(char *filename);
int vdseek(int fd, int offset, int whence);
int vdwrite(int fd, char *buffer, int bytes);
int vdread(int fd, char *buffer, int bytes);
int vdclose(int fd);
