#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argv, char** args) {

	// Open the file from arguments
	FILE *fp = fopen(args[1], "r");
	if(fp == NULL) {
		printf("Cannot open file\n");
		exit(1);
	}

	// Setup variables
	char command[20];
  int index = 1;
	int pid;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	// loop through the contents of the file
	read = getline(&line, &len, fp);
	do {
		// Trim the command
		command[strlen(command)-1]='\0';
		char arguments[10][10];
		int i, j, ctr=0;

		// Handle fork
		switch(pid = fork()) {
			case -1:				// FAILED
      	fprintf(stderr, "Fork Failed\n");
				exit(1);
			case 0: 				// CHILD
				// read the file
				for (int i=0; i<20; i++)
					for (int j=0; j<20; j++)
						arguments[i][j]=0;
				// split command into command and arguments
				for(i=0;i<=(strlen(command));i++) {
					if(command[i]==' '||command[i]=='\0') {
						arguments[ctr][j]='\0';
						ctr++;
						j=0;
					} else {
						arguments[ctr][j]=command[i];
						j++;
					}
				}
				char *myargs[3];
				myargs[0] = arguments[0];
				myargs[1] = arguments[1];
				if(strcmp(arguments[1], "") == 0) {
					myargs[1]=NULL;
				}
				myargs[2]=NULL;
				// execute the function
				execvp(myargs[0], myargs);
				_exit(0);
				break;
			default: 				// PARENT

				index++;
		}
	} while((wait(NULL) != -1) && ((read = getline(&line, &len, fp)) != -1));

	// Close the file and end program
	fclose(fp);
	return 0;
}
