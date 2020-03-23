#include <stdio.h>

int main(int argv, char** args) {

	int procs = argv - 1;

	for(int i = 1; i <= procs; i++) {
		printf("%s", args[i]);
	}

	return 0;
}