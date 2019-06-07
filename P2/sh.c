#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
	printf("sh > ");
	char command[100];
	scanf("%[^\n]%*c",command);
	int pid;
	while(strcmp(command,"exit") != 0 && strcmp(command,"shutdown") != 0){
		int flag = 0;
		// Checking for background order
		if(command[strlen(command)-1] == '&'){
			flag = 1;
		}
		pid = fork();
		if(pid == 0){
			char finalCommand[100];
			strncpy(finalCommand,command,sizeof(command));
			// If there's a background process call, then it removes the symbol to allow execution 
			if(flag == 1){ 
				strncpy(finalCommand,command,strlen(command)-2);
				finalCommand[strlen(command)-2] = '\0';
			}
			// Separating string commands
			char *separatedCommand[3], *original; 
			original = strdup(finalCommand);
			int j = 0; // Argument counter
			while((separatedCommand[j] = strsep(&original," ")) != NULL){
				//Increasing argument counter
				j++;
			}
			// Making call with correct arguments
			switch(j){
				case 1: 
					execlp(separatedCommand[0],separatedCommand[0],NULL);
					break;
				case 2:
					execlp(separatedCommand[0],separatedCommand[0],separatedCommand[1],NULL);
					break;
				case 3:
					execlp(separatedCommand[0],separatedCommand[0],separatedCommand[1],separatedCommand[2],NULL);
					break;
				default:
					execlp(separatedCommand[0],separatedCommand[0],NULL);
					break;
			}
			exit(0);
		}
		else{
			if(flag != 1){
				wait(NULL);
			}
			printf("\nsh > ");
			fflush(stdin);
			scanf("%[^\n]%*c",command);
		}
	}
	if(strcmp(command,"exit") == 0){
		exit(0); // Sends program termination status
	}
	else{ // Means command is shutdown
		exit(1); // Sends shutdown status
	}
}
	