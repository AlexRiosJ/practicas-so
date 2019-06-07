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
    int shutdown = -1;

    // Create all processes
    for(i = 0; i < CHILDREN; i++){
        if(getpid() == parent)
            pid = fork();
        if(pid == 0){
            // Exec with xterm
            execlp("xterm", "xterm", "-e", "./getty", NULL);
        }
    }

    // Wait for close or shutdown
    while(1) {
        wait(&status);
        shutdown = access("shutdown", F_OK);

        // If shutdown file exists, kill all xterm processes and return to terminate init
        if(shutdown != -1){
            remove("shutdown");
            execlp("killall", "killall", "xterm", NULL);
            return 0;
        }
        
        // If status is 0 create another xterm window
        if(status == 0) {
            if(getpid() == parent)
                pid = fork();
            if(pid == 0){
                execlp("xterm", "xterm", "-e", "./getty", NULL);
            }
        }
    }

    return 0;
}