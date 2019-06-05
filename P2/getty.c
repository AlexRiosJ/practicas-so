#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int validCredentials(char *user,char *pwd);

int main(){
	char user[50];
	char pwd[50];
	int pid;
	printf("Usuario:");
	scanf("%[^\n]%*c",user);
	printf("Contrasena:");
	scanf("%[^\n]%*c",pwd);	
	while(validCredentials(user,pwd) != 0){
		pid = fork();
		int status;
		if(pid == 0){ // Means if it's child process
			execlp("./sh","sh",NULL);
			exit(0);
		}
		else{
			wait(&status);
			if(WEXITSTATUS(status) == 0){
				printf("Usuario:");
				scanf("%[^\n]%*c",user);
				printf("Contrasena:");
				scanf("%[^\n]%*c",pwd);	
			}
			else{ // shutdown
				exit(1); // Sends shutdown state to init
			}
		}
	}
	exit(0);
}


int validCredentials(char *user,char *pwd){
	// Change implementation to verify if it exists on file
	return (strcmp(user,"Alfredo") == 0 && strcmp(pwd,"123") == 0);
	/*file = fopen("passwd.txt", "r");
	if (file) {
		while ((c = getc(file)) != EOF)
			putchar(c);
		fclose(file);
	}*/
}