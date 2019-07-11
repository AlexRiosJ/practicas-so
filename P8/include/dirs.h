typedef int VDDIR;
	

struct vddirent 
{
	char *d_name;
};

struct vddirent *vdreaddir(VDDIR *dirdesc);
VDDIR *vdopendir(char *path);
int vdclosedir(VDDIR *dirdesc);
