#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Same AVG method from before
int getSumAndCount(char *fileName, float *pointSum, int *pointCount) {
    char str[20];
    int count = 0;
    float sum = 0.0;

    // Open the file
    FILE *fp = fopen(fileName, "r");

    // File does not exist
    if(fp == NULL) {
        printf("cannot open file\n");
        exit(1);
    }

    // Loop through file
    while(fgets(str, 50, fp) != 0) {
        ++count;
        sum += atof(str);
    }

    // Print output
    *pointSum = sum;
    *pointCount = count;
    fclose(fp);
    return 0;
}


int main(int argc, char *argv[]) {
    int totalCount = 0;
    float totalSum = 0.0;

		// Create the pipes
		int sumPipe[2];
		int countPipe[2];

		// Open the pipes
		pipe(sumPipe);
		pipe(countPipe);

    // Loop through all argument file name
    for(int i = 1; i < argc; i++) {

        // Fork for each file
        int rc = fork();

        // Handle the fork
        if(rc < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0) {
            float localSum;
            int localCount;

						// Get Sum and Count for file
            getSumAndCount(argv[i], &localSum, &localCount);

						// Write to the pipes
						write(sumPipe[1], &localSum, sizeof(localSum));
						write(countPipe[1], &localCount, sizeof(localCount));

            exit(0);
        } else {
            wait(NULL);

						float localSum;
            int localCount;

						// Read from children
						read(sumPipe[0], &localSum, sizeof(localSum));
						read(countPipe[0], &localCount, sizeof(localCount));

						// Get the totals
						totalCount += localCount;
						totalSum += localSum;
        }
    }

		// Return AVG
		printf("%f\n", totalSum/totalCount);

		// Close our pipes
		close(sumPipe[0]);
		close(sumPipe[1]);
		close(countPipe[0]);
		close(countPipe[1]);

    return 0;
}
