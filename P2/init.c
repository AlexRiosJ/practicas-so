#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define CHILDREN 6

int main() {
    int pid;
    int i;

    for(i = 0; i < CHILDREN; i++){
        pid = fork();
        int status;
        if(pid == 0){
            execlp("xterm", "xterm", "-e", "./getty", NULL);
            exit(0);
        } else {
			wait(&status);
        }
    }

    return 0;
}