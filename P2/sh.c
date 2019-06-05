#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	printf("Hello Shell World!\n");
	printf("sh>");
	char command[100];
	scanf("%s[^\n]",command);
	int pid;
	while(strcmp(command,"exit") != 0 && strcmp(command,"shutdown") != 0){
		pid = fork();
		if(pid == 0){
			execlp(command,command,NULL);
			exit(0);
		}
		else{
			wait(NULL);
			printf("\nsh>");
			fflush(stdin);
			scanf("%s[^\n]",command);
		}
	}
	if(strcmp(command,"exit") == 0){
		exit(0); // Sends program termination status
	}
	else{ // Means command is shutdown
		exit(1); // Sends shutdown status
	}
}
	