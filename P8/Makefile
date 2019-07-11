I=include
B=bin
L=lib
S=sources
T=todo

all: $L/vdisk.o $L/bootsector.o $L/seclog.o $L/bitmaps.o $L/blocks.o $L/datetime.o $L/inode.o $L/fileslib.o $L/filesapi.o $L/users.o $L/dirs.o $B/createvd $B/dumpsec $B/dumpseclog $B/vdfdisk $B/vdformat $B/shell

$L/vdisk.o: $S/vdisk.c $I/vdisk.h
	gcc -c -o $L/vdisk.o $S/vdisk.c -I$I

$L/bootsector.o: $S/bootsector.c $I/bootsector.h
	gcc -c -o $L/bootsector.o $S/bootsector.c -I$I

$L/seclog.o: $S/seclog.c $I/seclog.h $I/vdisk.h
	gcc -c -o $L/seclog.o $S/seclog.c -I$I

$L/bitmaps.o: $S/bitmaps.c $I/bitmaps.h $I/bootsector.h $I/seclog.h
	gcc -c -o $L/bitmaps.o $S/bitmaps.c -I$I

$L/blocks.o: $S/blocks.c $I/blocks.h 
	gcc -c -o $L/blocks.o $S/blocks.c -I$I

$L/datetime.o: $S/datetime.c $I/datetime.h
	gcc -c -o $L/datetime.o $S/datetime.c -I$I

$L/inode.o: $S/inode.c $I/inode.h $I/bootsector.h $I/bitmaps.h $I/blocks.h $I/seclog.h $I/datetime.h
	gcc -c -o $L/inode.o $S/inode.c -I$I

$L/fileslib.o: $S/fileslib.c $I/filesys.h $I/bitmaps.h $I/inode.h
	gcc -c -o $L/fileslib.o $S/fileslib.c -I$I

$L/filesapi.o: $T/filesapi.c $I/filesapi.h $I/bootsector.h $I/seclog.h $I/bitmaps.h $I/blocks.h $I/inode.h $I/filesys.h $I/users.h 
	gcc -c -o $L/filesapi.o $T/filesapi.c -I$I
	
$L/users.o: $S/users.c $I/users.h
	gcc -c -o $L/users.o $S/users.c -I$I

$L/dirs.o: $S/dirs.c $I/dirs.h $I/bootsector.h $I/bitmaps.h $I/seclog.h $I/inode.h
	gcc -c -o $L/dirs.o $S/dirs.c -I$I

$B/createvd: $S/createvd.c $I/vdisk.h
	gcc -o $B/createvd $S/createvd.c -I$I

$B/dumpsec: $S/dumpsec.c $I/vdisk.h $L/vdisk.o
	gcc -o $B/dumpsec $S/dumpsec.c $L/vdisk.o -I$I

$B/dumpseclog: $S/dumpseclog.c $I/vdisk.h $I/seclog.h $L/vdisk.o $L/seclog.o
	gcc -o $B/dumpseclog $S/dumpseclog.c $L/vdisk.o $L/seclog.o -I$I

$B/vdfdisk: $S/dumpsec.c $I/vdisk.h $I/mbr.h $L/vdisk.o
	gcc -o $B/vdfdisk $S/vdfdisk.c $L/vdisk.o -I$I
	
$B/vdformat: $S/vdformat.c $I/vdisk.h $I/mbr.h $I/bootsector.h $I/seclog.h $I/bitmaps.h $L/vdisk.o $L/seclog.o $L/bitmaps.o $L/bootsector.o
	gcc -o $B/vdformat $S/vdformat.c $L/vdisk.o $L/seclog.o $L/bitmaps.o $L/bootsector.o -I$I
		
$B/shell: $S/shell.c $I/dirs.h $I/filesapi.h $L/dirs.o $L/users.o $L/filesapi.o $L/fileslib.o $L/inode.o $L/datetime.o $L/blocks.o $L/bitmaps.o $L/seclog.o $L/vdisk.o $L/bootsector.o
	gcc -o $B/shell $S/shell.c $L/dirs.o $L/users.o $L/filesapi.o $L/fileslib.o $L/inode.o $L/datetime.o $L/blocks.o $L/bitmaps.o $L/seclog.o $L/vdisk.o $L/bootsector.o  -I$I

clean:
	rm $L/vdisk.o $L/seclog.o $B/createvd $B/dumpsec $L/blocks.o $L/datetime.o $L/inode.o $L/fileslib.o $L/filesapi.o $L/users.o $L/dirs.o $L/bootsector.o $B/dumpseclog $B/vdfdisk $B/vdformat $L/bitmaps.o $B/shell 
	rm drives/*