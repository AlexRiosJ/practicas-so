#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mmu.h>

#define NUMPROCS 4
#define PAGESIZE 4096
#define PHISICALMEMORY 12 * PAGESIZE
#define TOTFRAMES PHISICALMEMORY / PAGESIZE
#define RESIDENTSETSIZE PHISICALMEMORY / (PAGESIZE * NUMPROCS)

extern char *base;
extern int framesbegin;
extern int idproc;
extern int systemframetablesize;
extern int ptlr;

extern struct SYSTEMFRAMETABLE *systemframetable;
extern struct PROCESSPAGETABLE *ptbr;

int getfreeframe();
int searchvirtualframe();
int getfifo();

int pagefault(char *vaddress)
{
    int i;
    int frame, vframe;
    long pag_a_expulsar;
    int fd;
    char buffer[PAGESIZE];
    int pag_del_proceso;

    // A partir de la dirección que provocó el fallo, calculamos la página
    pag_del_proceso = (long)vaddress >> 12;

    // Si la página del proceso está en un marco virtual del disco
    if (ptbr[pag_del_proceso].framenumber >= (systemframetablesize + framesbegin))
    {
        // Lee el marco virtual al buffer
        readblock(buffer, ptbr[pag_del_proceso].framenumber);
        // Libera el frame virtual
        systemframetable[ptbr[pag_del_proceso].framenumber].assigned = 0;
    }

    // Cuenta los marcos asignados al proceso
    i = countframesassigned();

    // Si ya ocupó todos sus marcos, expulsa una página
    if (i >= RESIDENTSETSIZE)
    {
        // Buscar una página a expulsar
        pag_a_expulsar = getfifo();
        // Poner el bit de presente en 0 en la tabla de páginas
        ptbr[pag_a_expulsar].presente = 0;
        // Si la página ya fue modificada, grábala en disco
        if (ptbr[pag_a_expulsar].modificado)
        {
            // Escribe el frame de la página en el archivo de respaldo y pon en 0 el bit de modificado
            saveframe(ptbr[pag_a_expulsar].framenumber);
            ptbr[pag_a_expulsar].modificado = 0;
        }

        // Busca un frame virtual en memoria secundaria
        vframe = searchvirtualframe();

        // Si no hay frames virtuales en memoria secundaria regresa error
        if (vframe == -1)
        {
            return (-1);
        }
        // Copia el frame a memoria secundaria, actualiza la tabla de páginas y libera el marco de la memoria principal
        copyframe(ptbr[pag_a_expulsar].framenumber, vframe);
        frame = ptbr[pag_a_expulsar].framenumber;
        systemframetable[frame].assigned = 0;
        ptbr[pag_a_expulsar].framenumber = vframe;
    }

    // Busca un marco físico libre en el sistema
    frame = getfreeframe();
    // Si no hay marcos físicos libres en el sistema regresa error
    if (frame == -1)
    {
        return (-1); // Regresar indicando error de memoria insuficiente
    }

    // Si la página estaba en memoria secundaria
    if (!ptbr[pag_del_proceso].presente)
    {
        // Cópialo al frame libre encontrado en memoria principal y transfiérelo a la memoria física
        copyframe(ptbr[pag_del_proceso].framenumber, frame);
        writeblock(buffer, frame);
        loadframe(frame);
    }

    // Poner el bit de presente en 1 en la tabla de páginas y el frame
    ptbr[pag_del_proceso].presente = 1;
    ptbr[pag_del_proceso].framenumber = frame;

    return (1); // Regresar todo bien
}

// Implementar funciones
int getfreeframe()
{
    int i;
    // Busca un marco libre en el sistema
    for (i = framesbegin; i < systemframetablesize + framesbegin; i++)
    {
        if (!systemframetable[i].assigned)
        {
            systemframetable[i].assigned = 1;
            break;
        }
    }

    return i < (systemframetablesize + framesbegin) ? i : -1;
}

int searchvirtualframe()
{
    int i;
    for (i = systemframetablesize + framesbegin; i < (systemframetablesize * 2 + framesbegin); i++)
    {
        if (!systemframetable[i].assigned)
        {
            systemframetable[i].assigned = 1;
            break;
        }
    }
    
    return i < (systemframetablesize * 2 + framesbegin) ? i : -1;
}

int getfifo()
{
    unsigned long min_time = 0xFFFFFFFFFFFFFFFF; 
    int temp_pag = -1;
    int i;

    for (i = 0; i < ptlr; i++)
    {
        if (ptbr[i].presente && ptbr[i].tarrived < min_time)
        {
            min_time = ptbr[i].tarrived;
            temp_pag = i;
        }
    }
    return temp_pag;
}
