#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define CHILDREN 6

int main() {
    int pid;
    int i;
    int status;
    int parent = getpid();

    for(i = 0; i < CHILDREN; i++){
        if(getpid() == parent)
            pid = fork();
        if(pid == 0){
            execlp("xterm", "xterm", "-e", "./getty", NULL);
        }
    }

    while(1) {
        wait(&status);
        printf("%d\n", status);
        if(status == 0) {
            if(getpid() == parent)
                pid = fork();
            if(pid == 0){
                execlp("xterm", "xterm", "-e", "./getty", NULL);
            }
        } else {
            return 0;
        }
    }

    return 0;
}